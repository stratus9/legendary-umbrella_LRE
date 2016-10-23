/*
 * SD.c
 *
 * Created: 2013-07-10 11:47:53
 *  Author: tmf
 */

#include "SD.h"
#include "FATFS/diskio.h"
#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>

uint8_t SD_Status;               //Co aktualnie jest podpi�te
uint8_t SD_Type;                 //Typ karty - SD lub SDHC

void SPI_SD_CS(_Bool SS_en) {
    if(SS_en) {
        SD_PORT.OUTCLR = SD_CS; //Aktywuj kart� SD
        SD_WaitForReady();       //Zaczekaj a� karta b�dzie gotowa
    } else {
        SD_PORT.OUTSET = SD_CS;
        SPI_RW_Byte(0xff);        //Karta ulega deaktywacji po otrzymaniu kolejnego zbocza sygna�u SCK
    }
}

void SPI_SetBaudrate_400kHz() {
    SD_SPI.CTRL = (SD_SPI.CTRL & (~SPI_PRESCALER_gm)) | SPI_PRESCALER_DIV64_gc; //Preskaler nale�y dobra� zale�nie od taktowania CPU
}

void SPI_SetMaxBaudrate() {
    SD_SPI.CTRL = (SD_SPI.CTRL & (~SPI_PRESCALER_gm)) | SPI_PRESCALER_DIV4_gc | SPI_CLK2X_bm;
}

uint8_t SPI_RW_Byte(uint8_t byte) {
    SD_SPI.DATA = byte;
    while(!(SD_SPI.STATUS & SPI_IF_bm));
    SD_SPI.STATUS = SPI_IF_bm;
    return SD_SPI.DATA;
}

void MemorySPIInit() {
    SD_PORT.DIRSET = SD_MOSI | SD_SCK | SD_CS | PIN4_bm;      //MOSI i SCK s� wyj�ciami
    SD_SPI.CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_0_gc | SPI_PRESCALER_DIV64_gc;
    SD_Status = STA_NOINIT; //Dysk niezainicjowany
}

void SD_CRC7(uint8_t *crc, uint8_t byte) { //Wylicza CRC7 dla karty, pocz�tkowo crc=0
    for (uint8_t i = 0; i < 8; i++) {
        *crc <<= 1;
        if ((((byte & 0x80) ^ (*crc & 0x80)) != 0))
            *crc ^= 0x09;
        byte <<= 1;
    }
}

uint8_t SD_SendCMD(uint8_t cmd, uint32_t arg) {
    uint8_t crc = 0, res;
    if(cmd & 0x80) {
        cmd &= 0x7F;          //Skasuj najstarszy bit polecenia
        res = SD_SendCMD(CMD55, 0); //Kolejne polecenie nale�y do grupy ACMD
        if(res > 0x01) return res; //Polecenie CMD55 zako�czy�o si� b��dem
    }
    SPI_SD_CS(false);         //Deaktywuj kart�
    SPI_SD_CS(true);          //Aktywuj kart�
    cmd |= 0x40;              //najstarsze dwa bity zawsze s� r�wne 01
    SPI_RW_Byte(cmd);
    SD_CRC7(&crc, cmd);
    for(uint8_t i = 0; i < 4; i++) {
        SPI_RW_Byte((arg >> 24) & 0xff);
        SD_CRC7(&crc, (arg >> 24) & 0xff);
        arg <<= 8;
    }
    crc = (crc << 1) | 1;      //CRC7 dla SD jest przesuni�te o jeden bit w lewo i ma ustawiony najm�odszy bit
    SPI_RW_Byte(crc);          //Wy�lij CRC polecenia
    uint8_t i = 10;       //Odpowied� mo�e nadej�� od 1 do 10 bajt�w po poleceniu
    do {
        res = SPI_RW_Byte(0xff);
    } while ((res & 0x80) && --i);
    return res;                //Po wys�aniu polecenia karta pozostaje wybrana w celu odczytu/zapisu kolejnych bajt�w
}

uint32_t SD_GetR7() { //Pobierz odpowied� typu R7
    uint32_t R7 = 0;
    for(uint8_t i = 0; i < 4; i++) {
        R7 <<= 8;
        R7 |= SPI_RW_Byte(0xff);
    }
    return R7;
}

_Bool SD_CardInitV1() {
    uint8_t i;
    for(i = 0; i < 255; i++) {
        if(SD_SendCMD(ACMD41, 0) == 0) break;  //Karta w tryb aktywny
        else _delay_ms(10);
    }
    if(i == 255) return false;      //Karta nie akceptuje polecenia - uszkodzona?
    SD_GetR7();                     //Co prawda mamy odpowied� R3, ale jej d�ugo�� jest taka sama jak R7
    SD_SendCMD(CMD16, 512);         //D�ugo�� bloku 512 bajt�w
    SD_Status = 0;
    SD_Type = CT_SD1;
    return true;
}

_Bool SD_CardInitV2() {
    uint8_t i;
    if(SD_GetR7() != 0x1aa)
        return false;  //B�edne napi�cie pracy karty
    for(i = 0; i < 255; i++) {
        if(SD_SendCMD(ACMD41, SD_OCR_CCS) == 0) break;  //Karta w tryb aktywny
        else _delay_ms(40);
    }
    if(i == 255) return false;           //Karta nie akceptuje polecenia - uszkodzona?
    if(SD_SendCMD(CMD58, 0) == 0) {
        SD_Status = 0;
        if(SD_GetR7() & SD_OCR_CCS) SD_Type = CT_SD2; //Pobierz OCR
        else SD_Type = CT_SD1;
    }
    return true;
}

_Bool SD_CardInit() {
    SPI_SetBaudrate_400kHz();     //Kart� nale�y zainicjowa� przy SCK r�wnym 400 kHz
    SPI_SD_CS(false);                      //Deaktywuj kart�
    for(uint8_t i = 0; i < 10; i++) {
        SPI_RW_Byte(0xff);                 //Wy�lij co najmniej 74 takty SCK dla inicjalizacji karty
    }
    uint8_t i = 10, ret;
    do {
        ret = SD_SendCMD(CMD0, 0);
    } while ((ret != SD_R1_Idle) && --i);  //Zaczekaj na prawid�ow� odpowied� ze strony karty
    if(!i) {
        SD_Status = STA_NODISK;            //Brak karty lub inny b��d
        return false;                      //Niedoczekali�my si�
    }
    i = 10;
    do {
        ret = SD_SendCMD(CMD8, 0x1aa);
    } while ((ret & 0xc0) && --i);
    _Bool result;
    if(ret == 0x05) result = SD_CardInitV1();
    else result = SD_CardInitV2(); //Z jak� kart� mamy do czynienia
    if(result == false) SD_Status = STA_NODISK;
    SPI_SetMaxBaudrate(&USARTC1);  //Prze��cz interfejs na maksymaln� szybko�� pracy
    return result;
}

_Bool SD_WaitForReady() {
    uint16_t counter = 5000;            //Czekamy maksymalnie ok. 500 ms
    SPI_RW_Byte(0xff);
    do {								//Zaczekaj na gotowo�� karty - sygna� CS musi by� wcze�niej aktywowany
        if(SPI_RW_Byte(0xff) == 0xff) break;
        _delay_us(100);
    } while (--counter);
    return counter ? true : false;      //Czy wyst�pi� timeout?
}

_Bool SD_Rec_Datablock(uint8_t *buff, uint16_t size) {
    uint8_t rec, timeout;
    timeout = 200;
    do {
        _delay_us(500);                  //Zanim blok danych b�dzie gotowy potrzebujemy chwilk�
        rec = SPI_RW_Byte(0xff);
    } while((rec == 0xff) && timeout--); //Czekamy na otrzymanie 0xfe, ale max 100 ms
    if(rec != 0xfe) return false;		 //B��d - nie ma pocz�tku danych
    do {						         //Odbierz blok danych
        *buff = SPI_RW_Byte(0xff);
        buff++;
    } while(--size);
    SPI_RW_Byte(0xff);                   //Odbierz CRC bloku danych - obowi�zkowe
    SPI_RW_Byte(0xff);
    return true;
}

_Bool SD_Sendblock(const uint8_t *buff, uint8_t token) { //Wy�lij 512-bajtowy blok danych i zako�cz go tokenem
    uint8_t resp;
    uint16_t counter;
    if(SD_WaitForReady() == false) return false; //Zaczekaj na gotowo�� karty
    SPI_RW_Byte(token);					//Wy�lij token pocz�tku/ko�ca danych
    if(token != 0xfd) {                 //0xfd to token ko�ca danych - po nim nic nie wysy�amy
        counter = 512;
        do {
            SPI_RW_Byte(*buff++);       //Wy�lij dane
        } while(--counter);
        SPI_RW_Byte(0xff);				//Musimy wys�a� CRC16, nawet je�li karta go nie sprawdza
        SPI_RW_Byte(0xff);
        resp = SPI_RW_Byte(0xff);			//Odczytaj odpoweid� karty
        if((resp & 0x1F) != 0x05) return false;	//Sprawd� czy karta zaakceptowa�a dane
    }
    return true;
}

uint8_t SD_disk_read(uint8_t *buff, uint32_t sector, uint8_t count) {
    if(SD_Status != 0) return RES_NOTRDY;
    if(SD_Type == CT_SD1) sector *= 512;	//SDSC adresowana jest bajtami
    if(count == 1) {
        //Odczytujemy pojedynczy sektor
        if((SD_SendCMD(CMD17, sector) == 0) && SD_Rec_Datablock(buff, 512))
            count = 0;
    } else {
        //Odczytujemy kilka sektor�w na raz
        if(SD_SendCMD(CMD18, sector) == 0) {
            do {
                if(!SD_Rec_Datablock(buff, 512)) break;
                buff += 512;
            } while(--count);
            SD_SendCMD(CMD12, 0);				//Koniec transmisji
        }
    }
    SPI_SD_CS(false);
    return count ? RES_ERROR : RES_OK;
}

uint8_t DS_disk_write(const uint8_t *buff, uint32_t sector, uint8_t count) {
    if(SD_Status != 0) return RES_NOTRDY;
    if(SD_Type == CT_SD1) sector *= 512;	//SDSC adresowana jest bajtami
    if(count == 1) {
        //Zapisujemy pojedynczy sektor
        if((SD_SendCMD(CMD24, sector) == 0) && SD_Sendblock(buff, 0xfe)) count = 0;
    } else {
        //Zapisujemy kilka sektor�w na raz
        SD_SendCMD(ACMD23, count);
        if(SD_SendCMD(CMD25, sector) == 0) {
            do {
                if (!SD_Sendblock(buff, 0xfc)) break;
                buff += 512;
            } while (--count);
            if (!SD_Sendblock(0, 0xfd)) count = 1;	//Token ko�ca wielobajtowego bloku danych
        }
    }
    SPI_SD_CS(false);
    return count ? RES_ERROR : RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE drv) {	/* Physical drive nmuber (0) */
    if (drv) return STA_NOINIT;			/* Supports only single drive */
    SD_CardInit();
    return SD_Status;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE drv) {	/* Physical drive nmuber (0) */
    if (drv) return STA_NOINIT;		/* Supports only single drive */
    return SD_Status;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE drv,			/* Physical drive nmuber (0) */
    BYTE *buff,			/* Pointer to the data buffer to store read data */
    DWORD sector,		/* Start sector number (LBA) */
    UINT count) {		/* Sector count (1..255) */
    if(drv) return RES_ERROR;
    return SD_disk_read(buff, sector, count);
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE drv,			/* Physical drive nmuber (0) */
    const BYTE *buff,	/* Pointer to the data to be written */
    DWORD sector,		/* Start sector number (LBA) */
    UINT count) {		/* Sector count (1..255) */
    if(drv) return RES_ERROR;
    return DS_disk_write(buff, sector, count);
}

_Bool SD_GetResponse(void *buf, uint8_t size) {
    uint8_t i = 10;
    while((SPI_RW_Byte(0xff) != 0xfe) && (--i));
    if(!i) return false;
    while(size) {
        ((uint8_t*)buf)[size - 1] = SPI_RW_Byte(0xff);
        --size;
    }
    return true;
}

DRESULT disk_ioctl (
    BYTE pdrv,		/* Physical drive nmuber (0..) */
    BYTE cmd,		/* Control code */
    void *buff) {	/* Buffer to send/receive control data */
    if(SD_Status != 0) return RES_NOTRDY;    //Pami�� niezainicjowana
    DRESULT res = RES_OK;
    switch(cmd) {
    case GET_SECTOR_SIZE:
        *((DWORD*)buff) = 512;       //D�ugo�� sektora 512 bajt�w
        break;
    case GET_SECTOR_COUNT: {
        char buf[16];
        SD_CSDRegv1 *csd = (SD_CSDRegv1*)buf;
        uint8_t r1 = SD_SendCMD(CMD9, 0); //Odczytaj CSD
        if(r1 == 0) {
            SD_GetResponse(csd->Reg, 16);
            uint64_t size;  //Rozmiar karty
            if(csd->CSD_Struct == 0)
                size = (csd->C_Size + 1) * (1ULL << csd->Read_Bl_Len) * (1UL << (csd->C_Size_mult + 2));
            else {
                size = (((SD_CSDRegv2*)csd)->C_Size + 1) * 524288ULL; //C_Size zawiera liczb� blok�w po 512 kB
            }
            *((DWORD*)buff) = size / 512;  //Zwracami liczb� 512-bajtowych sektor�w
        } else res = RES_NOTRDY;
    }
    break;
    case GET_BLOCK_SIZE:
        *((DWORD*)buff) = 1;         //Ile mo�na na raz skasowa� sektor�w
        break;
    case CTRL_SYNC:
        break;
    default:
        res = RES_PARERR;            //Nieznane polecenie
    }
    return res;
}



DWORD get_fattime (void) {
    /* Returns current time packed into a DWORD variable */
    return     ((DWORD)(2014 - 1980) << 25)   /* Year 2014 */
    | ((DWORD)1 << 21)            /* Month 1 */
    | ((DWORD)11 << 16)            /* Mday 28 */
    | ((DWORD)0 << 11)            /* Hour 0 */
    | ((DWORD)0 << 5)            /* Min 0 */
    | ((DWORD)0 >> 1);            /* Sec 0 */
}

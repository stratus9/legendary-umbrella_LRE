/*
 * SPI.h
 *
 * Created: 2015-05-15 11:45:41
 *  Author: stratus
 */ 


#ifndef SPI_H_
#define SPI_H_
#include <stdbool.h>
#include "struct.h"

#define FLASH_CS_pin   PIN4_bm       //Sygna³ CS karty SD
#define FLASH_PORT PORTE        //Port przez który pod³¹czona jest karta SD
#define FLASH_SPI  SPIE         //Alternatywnie port SPI do komunikacji

#define WREN        0x06    /* Write Enable */
#define WRDI        0x04    /* Write Disable */
#define RDSR        0x05    /* Read Status Register */
#define WRSR        0x01    /* Write Status Register */
#define READ        0x03    /* Read Data Bytes  3byte address*/
#define READ4       0x13    /* Read Data Bytes  4byte address*/
#define FAST_READ   0x0b    /* Read Data Bytes at Higher Speed //Not used as as the 328 isn't fast enough  */
#define PP          0x02    /* Page Program  3byte address*/
#define PP4         0x12    /* Page Program  4byte address*/
#define SE          0x20    /* Sector Erase (4k)  */
#define BE          0x20    /* Block Erase (64k)  */
#define CE          0xc7    /* Erase entire chip  */
#define DP          0xb9    /* Deep Power-down  */
#define RES         0xab    /* Release Power-down, return Device ID */
#define RDID        0x9F      /* Read Manufacture ID, memory type ID, capacity ID */


void SPI_W_Byte(uint8_t byte);
uint8_t SPI_R_Byte(void);

void FLASH_CS(uint8_t CS);
void FLASH_SetMaxBaudrate();
uint8_t FLASH_RW_Byte(uint8_t byte);
uint16_t FLASH_ReadID(void);
void FLASH_1byteCommand(uint8_t value);
void FLASH_2byteCommand(uint16_t value);
void FLASH_WriteEnable(uint8_t value);
uint8_t FLASH_ReadByte(uint32_t address);
void FLASH_arrayRead(uint32_t address, uint8_t * array, uint32_t length);
void FLASH_pageRead(uint32_t page, uint8_t * array, uint16_t length);
void FLASH_chipErase(void);
void FLASH_sectorErase(uint32_t address);
void FLASH_setup(void);
uint8_t FLASH_status(void);
void FLASH_waitForReady(void);
void FLASH_byteWrite(uint32_t address, uint8_t value);
void FLASH_pageWrite(uint32_t page, uint8_t * array, uint16_t length);
#endif /* SPI_H_ */

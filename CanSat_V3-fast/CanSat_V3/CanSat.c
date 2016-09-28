/*
 * CanSat_V3.c
 *
 * Created: 2015-03-22 16:38:24
 *  Author: stratus
 */


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <math.h>
#include "Initialization.h"
#include "struct.h"
#include "CanSat.h"
#include "util.h"
#include "SPI.h"
#include "ADC.h"
#include "I2C.h"


//-----------------------------------Struktury globalne---------------------------------------------
static Output_t Output_d;
static allData_t allData_d;
static RTC_t RTC_d;
static stan_t stan_d;
Analog_t Analog_d;
static frame_t frame_d;
static frame_t frame_b;
static buzzer_t buzzer_d;
static uint32_t SPIaddress = 0;
static float timer_buffer = 0;
uint32_t mission_time = 0;
uint32_t frame_count = 0;


//----------------------Bad ISR handling------------------------
ISR(BADISR_vect) {
    LED_PORT.OUTTGL = LED2;
}

//----------------------RTC ISR handling------------------------
/*
ISR(RTC_OVF_vect){
	frame_b.sec++;
	mission_time = frame_b.sec;
}*/


//----------------------Send to Xbee--------------------------------
ISR(USARTD0_TXC_vect) {
    if((frame_d.frameASCII[frame_d.iUART]) && (frame_d.frameASCII[frame_d.iUART] != '#')) {
        frame_d.mutex = true;
        if(frame_d.frameASCII[frame_d.iUART] == '%') XBEE_UART.DATA = '\n';
        else XBEE_UART.DATA = frame_d.frameASCII[frame_d.iUART];
        if(frame_d.iUART < 151) frame_d.iUART++;
        else frame_d.frameASCII[frame_d.iUART] = 0;
    } else frame_d.mutex = false;
}

//----------------------Receive from Xbee----------------------------
ISR(USARTD0_RXC_vect) {
    LED_PORT.OUTSET = LED3;
    char volatile tmp = XBEE_UART.DATA;
    if(tmp == '$') stan_d.cmd_mode = true;	//enter command mode
    else if((tmp == 'R') && stan_d.cmd_mode) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            stan_d.cmd_mode = false;
            CPU_CCP = CCP_IOREG_gc;
            RST.CTRL = RST_SWRST_bm;		//zdalny restart systemu
        }
    } else if((tmp == 'P') && stan_d.cmd_mode) {
        SPI_ChipErase();					//zerowanie pamiêci FLASH
        SPIaddress = 0;
        stan_d.cmd_mode = false;
        buzzer_d.mode = 1;								//3 sygna³y ci¹g³e
        buzzer_d.trigger = true;						//odblokowanie buzzera
		_delay_ms(1000);
    } else if((tmp == 'X') && stan_d.cmd_mode) {
        stan_d.flash_trigger = true;					//start zapisu do pamiêci FLASH
        stan_d.cmd_mode = false;
    } else if((tmp == 'K') && stan_d.cmd_mode) {
        stan_d.flash_trigger = false;					//koniec zapisu do pamiêci FLASH
        stan_d.cmd_mode = false;
    } else if((tmp == 'S') && stan_d.cmd_mode) {
        stan_d.telemetry_trigger = true;			//rozpoczêcie telemetrii
        stan_d.cmd_mode = false;
    } else if((tmp == 'F') && stan_d.cmd_mode) {
        stan_d.telemetry_trigger = false;			//koniec telemetrii
        stan_d.cmd_mode = false;
    } else if((tmp == 'A') && stan_d.cmd_mode) {
        stan_d.armed_trigger = true;				//uzbrojony
        stan_d.cmd_mode = false;
    } else if((tmp == 'D') && stan_d.cmd_mode) {
        stan_d.armed_trigger = false;				//rozbrojony
        stan_d.cmd_mode = false;
    } else stan_d.cmd_mode = false;
}

//----------------------Sensors update-------------------------------
ISR(TCC0_OVF_vect) {
    allData_d.stan->new_data = true;
    allData_d.RTC->time++;
}

//----------------------Buzzer---------------------------------------
ISR(TCD0_OVF_vect) {
    if(buzzer_d.trigger && BUZZER_ONOFF) {
        switch(buzzer_d.mode) {
        case 0:
            buzzer_d.trigger = false;
            buzzer_d.count = 0;
            buzzer_d.i = 0;
            PORTF.OUTCLR = PIN6_bm;
            break;
        case 1:
            if(buzzer_d.count > 6) buzzer_d.mode = 0;	//wy³¹czenie po 2 sygna³ach
            else if(buzzer_d.count <= 6) {
                PORTF.OUTTGL = PIN6_bm;
                buzzer_d.count++;
            }
            break;
        //-----------2Hz signal------------
        case 2:
            PORTF.OUTTGL = PIN6_bm;
            break;
        //----------cont signal------------
        case 3:
            PORTF.OUTSET = PIN6_bm;
            break;
        }
    } else {
        buzzer_d.mode = 0;
        PORTF.OUTCLR = PIN6_bm;
    }
}

//----------------------IO update-------------------------------------
ISR(TCE0_OVF_vect) {
    //-------------Blokada komend zdalnych----------------------------
    LED_PORT.OUTCLR = LED3;
    stan_d.cmd_mode = false;
}

//----------------------Frame send-------------------------------------
ISR(TCF0_OVF_vect) {
    LED_PORT.OUTTGL = LED4;
    frame_d.terminate = false;
    if(stan_d.telemetry_trigger) {
        if(RTC_d.frameTeleCount< 99999) RTC_d.frameTeleCount++;
        else RTC_d.frameTeleCount = 0;
        frame_b.iUART = 0;
        USARTD0_TXC_vect();
    }
}

//----------------------Memory erase---------------------------
void FLASHerase(void) {
    const char buf0[] = "\n\rMemory erased!\n\r\n\r\0";
    while((!(PORTE.IN & PIN0_bm)) || (!(PORTE.IN & PIN1_bm))) {}
    buzzer_d.mode = 3;																//sygna³ 2Hz
    buzzer_d.trigger = true;														//odblokowanie buzzera
    SPI_ChipErase();
    _delay_ms(1000);
    uint16_t i = 0;
    while(buf0[i]) {
        XBEE_UART.DATA = buf0[i++];
        _delay_ms(2);
    }
    buzzer_d.trigger = false;														//odblokowanie buzzera
    SPIaddress = 0;
}

void structInit(void) {
    frame_d.iUART = 0;
    stan_d.new_data = false;
    stan_d.new_frame = false;
	
	//----------------------Initialize allData_d--------------------
	allData_d.Analog = &Analog_d;
	allData_d.stan = &stan_d;
	allData_d.frame = &frame_d;
	allData_d.frame_b = &frame_b;
	allData_d.RTC = &RTC_d;
	allData_d.Output = &Output_d;
}

void SensorUpdate(allData_t * allData) {
    //-----------------AD7195 (1)--------------
	
	//-----------------AD7195 (2)--------------
    
    //-----------------Read ADC-------------
    AnalogUpdate(allData->Analog);
}

void StateUpdate(void) {
    if(!(stan_d.armed_trigger)) {
        stan_d.flightState = 0;
        buzzer_d.mode = 0;
		//stan_d.flash_trigger = false;
        PORTD_OUTCLR = PIN1_bm;
    } else {
        switch(stan_d.flightState) {
        //--------case 0 preflight-----------------
        case 0:
            //if((SensorData_d.accel_x > 3) || (LPS25H_d.velocity > 10)) stan_d.flightState = 1;	//wykrycie startu
            buzzer_d.mode = 0;
			//LPS25H_d.max_altitude = LPS25H_d.altitude;
            break;
        //--------case 1 flight wait for apogee----
        case 1:
			stan_d.flash_trigger = true;
            //if((LPS25H_d.max_altitude - LPS25H_d.altitude) > 10.0) stan_d.flightState = 2;	//wykrycie pu³apu
            break;
        //-------case 2 delay + sound signal + deployment------------------
        case 2:
            buzzer_d.mode = 1;																//3 sygna³y ci¹g³e
            buzzer_d.trigger = true;														//odblokowanie buzzera
            timer_buffer = RTC_d.time;														//buforowanie czasu
            stan_d.flightState = 3;
            break;
        //--------case 3 parachute delay--------------
        case 3:
            if(RTC_d.time > (timer_buffer + 5)) stan_d.flightState = 4;						//odczekanie po separacji
            break;
        //--------case 4 separation wait-----------
        case 4:
            //if(((LPS25H_d.velocity) > 40) || (LPS25H_d.altitude < 250)) stan_d.flightState = 5;						//odczekanie do separacji
            break;
        //-------case 5 separation------------------
        case 5:
            buzzer_d.mode = 1;																//3 sygna³y ci¹g³e
            buzzer_d.trigger = true;														//odblokowanie buzzera
            timer_buffer = RTC_d.time;														//buforowanie czasu
            stan_d.flightState = 6;
            break;
        //-------case 6 after separation delay------
        case 6:
            if(RTC_d.time > (timer_buffer + 10)) stan_d.flightState = 7;						//odczekanie po separacji
            break;
        //---------case 7 wait for landing----------
        case 7:
            //if((LPS25H_d.altitude < 100) && (LPS25H_d.velocity < 1) && (LPS25H_d.velocity > -1)) stan_d.flightState = 8;
            break;
        //---------case 8 END----------------
        case 8:
            buzzer_d.mode = 2;																//sygna³ 2Hz
            buzzer_d.trigger = true;														//odblokowanie buzzera
			stan_d.flash_trigger = false;
            break;
        }
    }
}

void Initialization(void) {
	CPU_clk(CPU_clock);	//zegar CPU
    OscRTC();			//zegar RTC
    RTC_Init();			//konfiguracja i uruchomienie RTC
    ADC_Init();			//inicjalizacja ADC
    USART_Init();		//inicjalizacja Xbee
    IO_Init();
    TimerCInit(sampling_time);	//sensor update
    TimerDInit(250);			//buzzer handling
    TimerEInit(50);				//obs³uga IO
    TimerFInit(telemetry_time);	//frame send
    structInit();
    I2C_Init();
    //--------AD7195 (1) Init-----------
	
	//--------AD7195 (2) Init-----------

    //-------SPI Flash Init--------
    SPI_Init();
    SPI_WriteProtection(false);
    SPIaddress = SPI_FindEnd();		//szukaj wolnego miejsca w pamiêci------------------------------------------------
    //SPIaddress = 0;
    //-------w³¹czenie przerwañ----
    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
}

void InitMemoryErase() {
    uint32_t i = 0;
    const char buf0[] = "\n\rMemory erased!\n\r\n\r\0";
    while((!(PORTE.IN & PIN0_bm)) || (!(PORTE.IN & PIN1_bm))) {}
    buzzer_d.mode = 3;																//sygna³ 2Hz
    buzzer_d.trigger = true;														//odblokowanie buzzera
    SPI_ChipErase();
    _delay_ms(1000);
    i = 0;
    while(buf0[i]) {
        XBEE_UART.DATA = buf0[i++];
        _delay_ms(2);
    }
    buzzer_d.trigger = false;														//odblokowanie buzzera
    SPIaddress = 0;
}

void InitMemoryRead() {
    uint32_t i = 0;
    stan_d.flash_trigger = false;
    buzzer_d.mode = 3;																//sygna³ 2Hz
    buzzer_d.trigger = true;
    _delay_ms(200);
    buzzer_d.trigger = false;
    while(!(PORTE.IN & PIN1_bm)) {}
    const char buf1[] = "\n\rTeam,TeleCnt,FlightState,SoftState,Altitude,Velocity,Accel,Gyro,Lat,Long,AltiGPS,Fix,Check,,Cnt,VoltageBat,VoltageVCC,Temp,Press,AccY,AccY2,GyroX,GyroZ,GyroY,\n\r\n\r\0";
    const char buf2[] = "\n\rReading done\n\r\n\r\0";
    //------Hello message----------
    while(buf1[i]) {
        XBEE_UART.DATA = buf1[i++];
        _delay_ms(10);
    }
    _delay_ms(1000);
    //-------start Flash read------S
    uint8_t ch = 0xFF;
    uint8_t FFcnt = 0;
    LED_PORT.OUTSET = LED1;
    SPI_CS(true);
    SPI_W_Byte(0x03);					//Read
    SPI_W_Byte(0);	//address MSB
    SPI_W_Byte(0);	//address cd.
    SPI_W_Byte(0);	//address LSB
    i = 0;
    do {
        ch = SPI_R_Byte();
        if(ch != 0xFF) {
            XBEE_UART.DATA = ch;
            FFcnt = 0;
        } else FFcnt++;
        i++;
        if((i % 100) == 0) _delay_ms(10);
        else _delay_us(100);
    } while((PORTE.IN & PIN0_bm) && (FFcnt < 100));
    SPI_CS(false);
    i = 0;
    while(buf2[i]) {
        XBEE_UART.DATA = buf2[i++];
        _delay_ms(1);
    }
}

void WarmUp() {
    //------Hello blink
    LED_PORT.OUTSET = LED2;
    _delay_ms(200);
    LED_PORT.OUTCLR = LED2;
    _delay_ms(100);
    LED_PORT.OUTSET = LED2;
    _delay_ms(200);
    LED_PORT.OUTCLR = LED2;
    _delay_ms(100);
}

void WarmUpMemoryOperations() {
    //----------------Kasowanie pamiêci Flash------------------------
    if((!(PORTE.IN & PIN0_bm)) && (!(PORTE.IN & PIN1_bm))) InitMemoryErase();
    //-----------------Odczyt z pamiêci i wys³anie po Xbee-----------
    else if(!(PORTE.IN & PIN1_bm)) InitMemoryRead();
}

int main(void) {
    stan_d.flash_trigger = STARTUP_flash;
    stan_d.telemetry_trigger = STARTUP_tele;
    frame_d.terminate = false;
    Initialization();
    sei();
    WarmUp();					//inicjalizacja BT i odmiganie startu
    WarmUpMemoryOperations();	//odczyt lub kasowanie pamiêci
	
    while(1) {
        _delay_us(1);
        if(stan_d.new_data == true) {
            //============================================================================
            //								Sensors update
            //============================================================================
            stan_d.new_data = false;	//DRY flag clear
            SensorUpdate(&allData_d);
			//============================================================================
			//						       State update
			//============================================================================
            StateUpdate();
			//============================================================================
            //                        Prepare frame & store
			//============================================================================
            prepareFrame(&allData_d);
            if(stan_d.flash_trigger) SPI_StoreFrame(&SPIaddress, 400, &frame_b,&RTC_d);
            if(!(frame_d.mutex)) frame_d = frame_b;	//jeœli frame_d nie zablokowane -> przepisz z bufora
            LED_PORT.OUTTGL = LED1;
        }
    }
}
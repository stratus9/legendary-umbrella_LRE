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
#include "I2C.h"
#include "AD7195.h"
#include "RealTime.h"

//----- do karty SD -----
#include <string.h>
#include "FATFS/ff.h"
#include "SD.h"
FIL pomiar;
FATFS fatfs;
UINT bw;


//-----------------------------------Struktury globalne---------------------------------------------
static Output_t Output_d;
static allData_t allData_d;
static AD7195_t AD7195_d;
static Clock_t Clock_d;
static stan_t stan_d;
Analog_t Analog_d;
static frame_t frame_d;
static frame_t frame_b;
static frameSD_t frame_sd;
static buzzer_t buzzer_d;
uint32_t mission_time = 0;
uint32_t frame_count = 0;

//----------------------Bad ISR handling------------------------
ISR(BADISR_vect) {
    LED_PORT.OUTTGL = LED1;
}

ISR(ADCB_CH3_vect){
	Analog_d.AnalogIn1 = ADCB.CH0RES;	//dodaæ przeliczenie na temp lub zrobiæ to w frame_prepare
	Analog_d.AnalogIn2 = ADCB.CH1RES;
	Analog_d.AnalogIn3 = ADCB.CH2RES;
	Analog_d.AnalogIn4 = ADCB.CH3RES;
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
        if(frame_d.frameASCII[frame_d.iUART] == '%') USARTD0.DATA = '\n';
        else USARTD0.DATA = frame_d.frameASCII[frame_d.iUART];
        if(frame_d.iUART < 151) frame_d.iUART++;
        else frame_d.frameASCII[frame_d.iUART] = 0;
    } else frame_d.mutex = false;
}

//----------------------Receive from Xbee----------------------------
ISR(USARTD0_RXC_vect) {
    //LED_PORT.OUTSET = LED3;
    char volatile tmp = USARTD0.DATA;
    if(tmp == '$') stan_d.cmd_mode = true;	//enter command mode
	
	//------- Reset --------
    else if((tmp == 'R') && stan_d.cmd_mode) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            stan_d.cmd_mode = false;
            CPU_CCP = CCP_IOREG_gc;
            RST.CTRL = RST_SWRST_bm;		//zdalny restart systemu
        }
	
	//-----Test MOV------
    } else if((tmp == '1') && stan_d.cmd_mode) {
        MOV_valve_open();
		_delay_ms(1000);
		MOV_valve_close();						
        stan_d.cmd_mode = false;
		
	//-----Test MFV------
    } else if((tmp == '2') && stan_d.cmd_mode) {
        MFV_valve_open();
		_delay_ms(1000);
		MFV_valve_close();				
        stan_d.cmd_mode = false;
		
	//-----Test MPV------
    } else if((tmp == '3') && stan_d.cmd_mode) {
        MPV_valve_open();
		_delay_ms(1000);
		MPV_valve_close();			
        stan_d.cmd_mode = false;
		
	//-----Test FPV------
    } else if((tmp == '4') && stan_d.cmd_mode) {
        FPV_valve_open();
		_delay_ms(1000);
		FPV_valve_close();
        stan_d.cmd_mode = false;
		
	//------Konfiguracja sekwencji 1------ dzia³a
    } else if((tmp == '6') && stan_d.cmd_mode) {
		stan_d.TestConfig = 150;		
		stan_d.cmd_mode = false;
		
	//------Konfiguracja sekwencji 2-------- dzia³a
    } else if((tmp == '7') && stan_d.cmd_mode) {
		stan_d.TestConfig = 200;		
		stan_d.cmd_mode = false;
		
	//------Konfiguracja sekwencji 3--------- dzia³a
    } else if((tmp == '8') && stan_d.cmd_mode) {
		stan_d.TestConfig = 350;		
		stan_d.cmd_mode = false;
		
	//------Konfiguracja sekwencji 4--------- dzia³a
    } else if((tmp == '9') && stan_d.cmd_mode) {
		stan_d.TestConfig = 650;		
		stan_d.cmd_mode = false;
		
	//------Przerwanie testu----------------- dzia³a
    } else if((tmp == 'A') && stan_d.cmd_mode) {
        stan_d.Abort = true;						//ABORT!!!! ABORT!!! ABORT!!!
		stan_d.run_trigger = false;
		stan_d.armed_trigger = false;
        stan_d.cmd_mode = false;
		
	//------Doprê¿anie ---------------- 
    } else if((tmp == 'P') && stan_d.cmd_mode) {
        stan_d.armed_trigger = true;				
        stan_d.cmd_mode = false;
		char filename[8];
		FindNextFilename(filename);
		if (f_open(&pomiar, filename, FA_WRITE | FA_CREATE_ALWAYS) == FR_OK){	//jesli plik "naszplik.txt" nie istnieje, stworz go
			//f_write(&pomiar, "State, Config, Press 1, Press 2, Press 3, Temp 1, Temp 2, Temp3, Press 4, Press 5, Press 6, Temp 4\r", 101, &bw);
			f_write(&pomiar, "Press 1, Press 2, Press 3, Press 4, Press 5, Press 6, Press 7, Press 8, Temp 1, Temp 2, Temp3, , Temp 4, Count\r", 111, &bw);
		}
	
	//------ Rozpoczêcie testu --------
	} else if((tmp == 'S') && (stan_d.TestConfig != 0) && stan_d.cmd_mode) {
	stan_d.run_trigger = true;
	stan_d.cmd_mode = false;
	
    } else stan_d.cmd_mode = false;
}

//----------------------Sensors update-------------------------------
ISR(TCC0_OVF_vect) {
     
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
    //LED_PORT.OUTCLR = LED3;
    stan_d.cmd_mode = false;
	Clock_d.time++;
}

//----------------------Frame send-------------------------------------
ISR(TCF0_OVF_vect) {
    LED_PORT.OUTTGL = LED4;
    frame_d.terminate = false;
    if(stan_d.telemetry_trigger) {
        if(Clock_d.frameTeleCount< 99999) Clock_d.frameTeleCount++;
        else Clock_d.frameTeleCount = 0;
        frame_b.iUART = 0;
        USARTD0_TXC_vect();
    }
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
	allData_d.Clock = &Clock_d;
	allData_d.Output = &Output_d;
	allData_d.AD7195 = &AD7195_d;
}

void SensorUpdate(allData_t * allData) {
    //-----------------AD7195 (1)--------------
	
	//-----------------AD7195 (2)--------------
    
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
    TimerEInit(10);				//Obs³uga RTC
    TimerFInit(telemetry_time);	//frame send
    structInit();
    I2C_Init();
	SPI_Init();
    //--------AD7195 (1) Init-----------
	AD7195_Reset(0);
	AD7195_Init(0);
	//volatile char id1 = AD7195_WhoIam(0);
	//--------AD7195 (2) Init-----------
	AD7195_Reset(1);
	AD7195_Init(1);
	//volatile char id2 = AD7195_WhoIam(1);
	//------- AD7195 Sync ------------
	AD7195_Sync();
    //-------SPI Memory Init--------
	MemorySPIInit();
	SD_CardInit();
    //-------w³¹czenie przerwañ----
    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
}

void WarmUp() {
    //------Hello blink
    LED_PORT.OUTSET = LED3;
    _delay_ms(200);
    LED_PORT.OUTCLR = LED3;
    _delay_ms(100);
    LED_PORT.OUTSET = LED3;
    _delay_ms(200);
    LED_PORT.OUTCLR = LED3;
    _delay_ms(100);
}

int main(void) {
    stan_d.flash_trigger = STARTUP_flash;
    stan_d.telemetry_trigger = STARTUP_tele;
    frame_d.terminate = false;
    Initialization();
    sei();
    WarmUp();					//odmiganie startu
    
	uint32_t timer_buffer = 0;
	uint8_t counter = 0;
	
	f_mount(&fatfs,0,1);  //Dostêp do systemu plików
	Light_Green();
    while(1){
        _delay_us(1);
//============================== Sekcja pomiarów ============================================
		if(!AD7195_RDY(0)){
			LED_PORT.OUTTGL = LED5;
			ADC_tempCalc(&Analog_d);
			AD7195_ReadStore(&allData_d);
			if(counter >= 3){
				counter = 0;
				AD7195_PressureCalc(&AD7195_d);
				prepareFrameDEBUG(&allData_d);
				if(!frame_d.mutex) frame_d = frame_b;
				LED_PORT.OUTSET = LED6;
				if(stan_d.armed_trigger){
					Clock_d.frameFlashCount++;
					if(Add2Buffer(&frame_b, &frame_sd) >= 450){
						LED_PORT.OUTSET = LED2;
						f_write(&pomiar, &frame_sd, FindTableLength(frame_sd.frameASCII), &bw);
						//f_write(&pomiar, &frame_sd, 500, &bw);
						frame_sd.frameASCII[0] = 0;
						LED_PORT.OUTCLR = LED2;
					}
				}
				else f_close(&pomiar);
				LED_PORT.OUTCLR = LED6;
			}
			else counter++;
		}

//=========================== Sekcja maszyny stanów =========================================
		CheckOutputState(&stan_d);
		if(stan_d.Abort == true){
			SERVO_close();
			FPV_valve_close();
			MFV_valve_close();
			MOV_valve_close();
			MPV_valve_open();
			Buzzer_active();
			Light_Red();
			stan_d.TestConfig = 0;
			PORTF.OUTSET = PIN0_bm;
			LED_PORT.OUTCLR = LED3;
		}
		else if((stan_d.armed_trigger == true) && (stan_d.run_trigger == false)) FPV_valve_open();	//w³¹czenie doprê¿ania
        else if((stan_d.armed_trigger == true) && (stan_d.run_trigger == true) && (stan_d.TestConfig != 0)) {
			if(timer_buffer <= Clock_d.time){
				stan_d.State++;
				switch(stan_d.State){
					case 0:
					LED_PORT.OUTSET = LED3;
					stan_d.State = 1;
					break;
					//----- Step 1---------------- rozruch
					case 1:
					Buzzer_active();
					SERVO_close();
					Light_Red();
					timer_buffer = Clock_d.time+1000;
					break;
					//----- Step 2---------------- zapalnik
					case 2:
					Buzzer_inactive();
					Ignition_active();
					timer_buffer = Clock_d.time+200;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 5s
					break;
					//-----Step 3----------------- otwarcie paliwa i n2o
					case 3:
					Ignition_inactive();
					MFV_valve_open();
					MOV_valve_open();
					SERVO_open();
					timer_buffer = Clock_d.time+stan_d.TestConfig;
					break;
					//-----Step 4a---------------- zamkniêcie n2o
					case 4:
					MOV_valve_close();
					timer_buffer = Clock_d.time+10;
					break;
					//-----Step 4b---------------- zamkniêcie paliwa
					case 5:
					MFV_valve_close();
					FPV_valve_close();
					SERVO_close();
					MPV_valve_open();	//gaszenie
					timer_buffer = Clock_d.time+1000;
					break;
					//-----Step 5------------------
					case 6:
					default:
					MPV_valve_close();
					Light_Green();
					stan_d.run_trigger = false;
					stan_d.armed_trigger = false;
					stan_d.TestConfig = 0;
					stan_d.State = 0;
					LED_PORT.OUTCLR = LED3;
					break;
				}
			}
        }
    }
}
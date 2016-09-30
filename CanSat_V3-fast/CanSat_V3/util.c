/*
 * util.c
 *
 * Created: 2015-03-22 20:23:40
 *  Author: stratus
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <util/delay.h>
#include "struct.h"
#include "util.h"
#include "CanSat.h"

void float2char(float number,char * tablica){
	uint16_t tmp;
	if(number < 0){
		*(tablica) = '-';
		tmp = 10*(-number);
	}
	else{
		*(tablica) = '+';
		tmp = 10*number;
	}
	*(tablica+1) = ((tmp%100000UL)/10000UL) + 48;
	*(tablica+2) = ((tmp%10000UL)/1000UL) + 48;
	*(tablica+3) = ((tmp%1000UL)/100) + 48;
	*(tablica+4) = ((tmp%100)/10) + 48;
	*(tablica+5) = '.';
	*(tablica+6) = ((tmp%10)) + 48;
}

void prepareFrame(allData_t * allData){
	volatile int16_t i,tmp,tmpf;
	i=0;
	//------------ state-------------------------
	allData->frame_b->frameASCII[i++] = 'S';
	allData->frame_b->frameASCII[i++] = allData->stan->run_trigger+48;	//1
	allData->frame_b->frameASCII[i++] = allData->stan->FPV+48;			//2
	allData->frame_b->frameASCII[i++] = allData->stan->MFV+48;			//3
	allData->frame_b->frameASCII[i++] = allData->stan->MOV+48;			//4
	allData->frame_b->frameASCII[i++] = allData->stan->MPV+48;			//5
	allData->frame_b->frameASCII[i++] = 0;		
	allData->frame_b->frameASCII[i++] = 0;
	allData->frame_b->frameASCII[i++] = allData->stan->IGN+48;			//8
	allData->frame_b->frameASCII[i++] = allData->stan->SERVO1+48;		//9
	allData->frame_b->frameASCII[i++] = allData->stan->SERVO2+48;		//10
	allData->frame_b->frameASCII[i++] = ',';
	allData->frame_b->frameASCII[i++] = allData->stan->TestConfig+48;	//konfiguracja testu
	allData->frame_b->frameASCII[i++] = ',';
	
	//===============Pressure & altitude===================
	//--------------LPS25H Altitude----------------------------
	//tmp = allData->SensorsData->altitude;
	//tmpf = (allData->SensorsData->altitude - truncf(allData->SensorsData->altitude))*100;
	if((tmp < 0) || (tmpf <0)){
		tmp = -tmp;
		tmpf = -tmpf;
		allData->frame_b->frameASCII[i++] = '-';
	}
	else allData->frame_b->frameASCII[i++] = '+';
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '.';
	allData->frame_b->frameASCII[i++] = (tmpf/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmpf/1)%10 + 48;
	allData->frame_b->frameASCII[i++] = ',';
	
	allData->frame_b->frameASCII[i++] = '\n';		
	//----------------packet count-----------------------
	tmp = allData->RTC->frameTeleCount;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = ',';
	allData->frame_b->frameASCII[i++] = '\r';
	allData->frame_b->frameASCII[i++] = '\n';		
	allData->frame_b->frameASCII[i++] = 0;
	allData->frame_b->frameASCII[i++] = 'X';
}

//	IGN - RELAY1 - PE2
//	MFV - RELAY2 - PE1
//	MOV - RELAY3 - PE0
//	MPV - RELAY4 - PR0
//	WPV	- RELAY5 - PR1
//	??? - RELAY6 - PA6
//	LGH - RELAY7 - PA7

//============== Zapalnik ==========================
void Ignition_active(void){
	PORTE.OUTSET = PIN2_bm;
}
void Ignition_inactive(void){
	PORTE.OUTCLR = PIN2_bm;
}

//========== G³ówny zawór paliwa ===================
void MFV_valve_open(void){
	PORTE.OUTSET = PIN1_bm;
}
void MFV_valve_close(void){
	PORTE.OUTCLR = PIN1_bm;
}

//========== G³ówny zawór utleniacza ===============
void MOV_valve_open(void){
	PORTE.OUTSET = PIN0_bm;
}
void MOV_valve_close(void){
	PORTE.OUTCLR = PIN0_bm;
}

//========== G³ówny zawór wody =====================
void MPV_valve_open(void){
	PORTR.OUTSET = PIN0_bm;
}
void MPV_valve_close(void){
	PORTR.OUTCLR = PIN0_bm;
}

//========== G³ówny zawór doprê¿ania ===============
void FPV_valve_open(void){
	PORTR.OUTSET = PIN1_bm;
}
void FPV_valve_close(void){
	PORTR.OUTCLR = PIN1_bm;
}

//==================== SERVO =======================
void SERVO_open(void){
	PORTD.OUTSET = PIN4_bm;
	PORTD.OUTCLR = PIN5_bm;
}
void SERVO_close(void){
	PORTD.OUTCLR = PIN4_bm;
	PORTD.OUTSET = PIN5_bm;
}

//==================== Buzzer =======================
void Buzzer_active(void){
	PORTR.OUTSET = PIN1_bm;
}
void Buzzer_inactive(void){
	PORTR.OUTCLR = PIN1_bm;
}

//==================== LIGHT =======================
void Light_Green(void){
	PORTA.OUTSET = PIN7_bm;
}
void Light_Red(void){
	PORTA.OUTCLR = PIN7_bm;
}

void ADC_sync(void){
	PORTD.OUTCLR = PIN1_bm;						//konfiguracja stanu pinu buzzer
	_delay_ms(100);
	PORTD.OUTSET = PIN1_bm;						//konfiguracja stanu pinu buzzer
}

void BL_onoff(bool state){
	if(state) PORTC.OUTSET = PIN2_bm;						
	else PORTC.OUTCLR = PIN2_bm;						
}

void CheckOutputState(stan_t * stan){
	stan->MOV = PORTE.IN & PIN0_bm;
	stan->MFV = PORTE.IN & PIN1_bm;
	stan->MPV = PORTR.IN & PIN0_bm;
	stan->FPV = PORTR.IN & PIN1_bm;
	stan->IGN = PORTE.IN & PIN2_bm;
	stan->SERVO1 = PORTD.IN & PIN5_bm;
	stan->SERVO2 = PORTD.IN & PIN4_bm;
}
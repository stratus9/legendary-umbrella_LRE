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
#include "FATFS/ff.h"

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
void prepareFrameDEBUG(allData_t * allData){
	volatile int32_t i,tmp,tmpf;
	i=0;
	
	//=============== Pressure ===================
	//-------------- Pressure 1 ----------------------------
	tmp = allData->AD7195->raw_press1;	//bar
	if(tmp < 0) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Pressure 2 ----------------------------
	tmp = allData->AD7195->raw_press2;	//bar
	if(tmp < 0) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Pressure 3 ----------------------------
	tmp = allData->AD7195->raw_press3;	//bar
	if((tmp < 0) || (tmpf <0)) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Pressure 4 ----------------------------
	tmp = allData->AD7195->raw_press4;	//bar
	if(tmp < 0) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Pressure 5 ----------------------------
	tmp = allData->AD7195->raw_press5;	//bar
	if(tmp < 0) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Pressure 6 ----------------------------
	tmp = allData->AD7195->raw_press6;	//bar
	if(tmp < 0) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Pressure 7 ----------------------------
	tmp = allData->AD7195->raw_press7;	//bar
	if(tmp < 0) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Pressure 8 ----------------------------
	tmp = allData->AD7195->raw_press8;	//bar
	if(tmp < 0) tmp = -tmp;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Temp 1 ----------------------------
	tmp = allData->Analog->AnalogIn1;
	if(tmp < 0){
		tmp = -tmp;
		allData->frame_b->frameASCII[i++] = '-';
	}
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Temp 2 ----------------------------
	tmp = allData->Analog->AnalogIn2;
	if(tmp < 0){
		tmp = -tmp;
		allData->frame_b->frameASCII[i++] = '-';
	}
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Temp 3 ----------------------------
	tmp = allData->Analog->AnalogIn3;
	if(tmp < 0){
		tmp = -tmp;
		allData->frame_b->frameASCII[i++] = '-';
	}
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//-------------- Temp 4 ----------------------------
	tmp = allData->Analog->AnalogIn4;
	if(tmp < 0){
		tmp = -tmp;
		allData->frame_b->frameASCII[i++] = '-';
	}
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = '\t';
	
	//----------------packet count-----------------------
	tmp = allData->Clock->frameFlashCount;
	allData->frame_b->frameASCII[i++] = (tmp/10000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	
	//------------ END --------------
	allData->frame_b->frameASCII[i++] = '\r';
	allData->frame_b->frameASCII[i++] = '\n';
	allData->frame_b->frameASCII[i++] = 0;
	allData->frame_b->frameASCII[i++] = 'X';
}

void prepareFrame(allData_t * allData){
	volatile int16_t i,tmp,tmpf;
	i=0;
	
	/* 1  */	//------------ state-------------------------
	/* 2  */	allData->frame_b->frameASCII[i++] = 'S';
	/* 3  */	allData->frame_b->frameASCII[i++] = allData->stan->run_trigger+48;	//1
	/* 4  */	allData->frame_b->frameASCII[i++] = allData->stan->FPV+48;			//2
	/* 5  */	allData->frame_b->frameASCII[i++] = allData->stan->MFV+48;			//3
	/* 6  */	allData->frame_b->frameASCII[i++] = allData->stan->MOV+48;			//4
	/* 7  */	allData->frame_b->frameASCII[i++] = allData->stan->MPV+48;			//5	
	/* 8  */	allData->frame_b->frameASCII[i++] = '0';							//6
	/* 9  */	allData->frame_b->frameASCII[i++] = allData->stan->IGN+48;			//7
	/* 10 */	allData->frame_b->frameASCII[i++] = allData->stan->SERVO1+48;		//8
	/* 11 */	allData->frame_b->frameASCII[i++] = allData->stan->SERVO2+48;		//9
	/* 12 */	allData->frame_b->frameASCII[i++] = ',';
	/* 13 */	allData->frame_b->frameASCII[i++] = (allData->stan->TestConfig/1000)%10+48;	//konfiguracja testu
	/* 14 */	allData->frame_b->frameASCII[i++] = (allData->stan->TestConfig/100)%10+48;	//konfiguracja testu
	/* 15 */	allData->frame_b->frameASCII[i++] = (allData->stan->TestConfig/10)%10+48;	//konfiguracja testu
	/* 16 */	allData->frame_b->frameASCII[i++] = (allData->stan->TestConfig)%10+48;		//konfiguracja testu
	/* 17 */	allData->frame_b->frameASCII[i++] = ',';
	
	//=============== Pressure ===================
	//-------------- Pressure 4 ----------------------------
	tmp = allData->AD7195->pressure4*1000;	//bar
	if(tmp < 0) tmp = -tmp;
	/* 18 */	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	/* 19 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 21 */	allData->frame_b->frameASCII[i++] = '.';
	/* 22 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 23 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 24 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 25 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Pressure 5 ----------------------------
	tmp = allData->AD7195->pressure5*1000;	//bar
	if(tmp < 0) tmp = -tmp;
	/* 26 */	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	/* 27 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 28 */	allData->frame_b->frameASCII[i++] = '.';
	/* 29 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 30 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 31 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 32 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Pressure 6 ----------------------------
	tmp = allData->AD7195->pressure6*1000;	//bar
	if((tmp < 0) || (tmpf <0)) tmp = -tmp;
	/* 33 */	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	/* 34 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 35 */	allData->frame_b->frameASCII[i++] = '.';
	/* 36 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 37 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 38 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 39 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Temp 1 ----------------------------
	tmp = allData->Analog->Temp1*10;
	if(tmp < 0){
		tmp = -tmp;
		tmpf = -tmpf;
		allData->frame_b->frameASCII[i++] = '-';
	}
	/* 41 */	else allData->frame_b->frameASCII[i++] = '+';
	/* 42 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 43 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 44 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 45 */	allData->frame_b->frameASCII[i++] = '.';
	/* 46 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 47 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Temp 2 ----------------------------
	tmp = allData->Analog->Temp2*10;
	if(tmp < 0){
		tmp = -tmp;
		tmpf = -tmpf;
		allData->frame_b->frameASCII[i++] = '-';
	}
	/* 48 */	else allData->frame_b->frameASCII[i++] = '+';
	/* 49 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 50 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 51 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 52 */	allData->frame_b->frameASCII[i++] = '.';
	/* 53 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 54 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Temp 3 ----------------------------
	tmp = allData->Analog->Temp3*10;
	if(tmp < 0){
		tmp = -tmp;
		tmpf = -tmpf;
		allData->frame_b->frameASCII[i++] = '-';
	}
	/* 55 */	else allData->frame_b->frameASCII[i++] = '+';
	/* 56 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 57 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 58 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 59 */	allData->frame_b->frameASCII[i++] = '.';
	/* 60 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 61 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Temp 4 ----------------------------
	tmp = allData->Analog->Temp4*10;
	if(tmp < 0){
		tmp = -tmp;
		tmpf = -tmpf;
		allData->frame_b->frameASCII[i++] = '-';
	}
	/* 62 */	else allData->frame_b->frameASCII[i++] = '+';
	/* 63 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 64 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 65 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 66 */	allData->frame_b->frameASCII[i++] = '.';
	/* 67 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 68 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Pressure 7 ----------------------------
	tmp = allData->AD7195->pressure7*1000;	//bar
	if(tmp < 0) tmp = -tmp;
	/* 69 */	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	/* 70 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 71 */	allData->frame_b->frameASCII[i++] = '.';
	/* 72 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 73 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 74 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 75 */	allData->frame_b->frameASCII[i++] = ',';
	
	//-------------- Pressure 8 ----------------------------
	tmp = allData->AD7195->pressure8*1000;	//bar
	if(tmp < 0) tmp = -tmp;
	/* 76 */	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	/* 77 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 78 */	allData->frame_b->frameASCII[i++] = '.';
	/* 79 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 80 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 81 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	/* 82 */	allData->frame_b->frameASCII[i++] = ',';
	
	//----------------packet count-----------------------
	tmp = allData->Clock->frameFlashCount;
	/* 83 */	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	/* 84 */	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	/* 85 */	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	/* 86 */	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	/* 87 */	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	
	//------------ END --------------
	allData->frame_b->frameASCII[i++] = '\r';
	allData->frame_b->frameASCII[i++] = '\n';		
	allData->frame_b->frameASCII[i++] = 0;
	allData->frame_b->frameASCII[i++] = 'X';
}

//	IGN - RELAY1 - PE2
//	MFV - RELAY2 - PE1
//	MOV - RELAY3 - PE0
//	MPV - RELAY4 - PR0
//	FPV	- RELAY5 - PR1
//	??? - RELAY6 - PA6
//	LGH - RELAY7 - PA7

//============== Zapalnik ==========================
void Ignition_active(void){
	PORTE.OUTSET = PIN2_bm;
}
void Ignition_inactive(void){
	PORTE.OUTCLR = PIN2_bm;
}

//========== G��wny zaw�r paliwa ===================
void MFV_valve_open(void){
	PORTE.OUTSET = PIN1_bm;
}
void MFV_valve_close(void){
	PORTE.OUTCLR = PIN1_bm;
}

//========== G��wny zaw�r utleniacza ===============
void MOV_valve_open(void){
	PORTE.OUTSET = PIN0_bm;
}
void MOV_valve_close(void){
	PORTE.OUTCLR = PIN0_bm;
}

//========== G��wny zaw�r wody =====================
void MPV_valve_open(void){
	PORTR.OUTSET = PIN0_bm;
}
void MPV_valve_close(void){
	PORTR.OUTCLR = PIN0_bm;
}

//========== G��wny zaw�r dopr�ania ===============
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
	PORTD.OUTSET = PIN0_bm;
}
void Buzzer_inactive(void){
	PORTD.OUTCLR = PIN0_bm;
}

//==================== LIGHT =======================
void Light_Green(void){
	PORTA.OUTSET = PIN7_bm;
}
void Light_Red(void){
	PORTA.OUTCLR = PIN7_bm;
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

void TempConvPT100(Analog_t * Analog){
	Analog->Temp1 = 0;
}

uint16_t Add2Buffer(frame_t * source, frameSD_t * destination){
	volatile uint16_t i=0, j=0;
	while(destination->frameASCII[j]) j++;
	while(source->frameASCII[i]) destination->frameASCII[j++] = source->frameASCII[i++];
	destination->frameASCII[j++] = 0;
	return j;
}

void ADC_tempCalc(Analog_t * Analog){
	Analog->Temp1 = 2.616979*220.0*(Analog->AnalogIn1)/2048.0*3.0 / (5.0-(Analog->AnalogIn1)/2048.0*3.0) - 260.1696;
	Analog->Temp2 = 2.616979*220.0*(Analog->AnalogIn2)/2048.0*3.0 / (5.0-(Analog->AnalogIn2)/2048.0*3.0) - 260.1696;
	Analog->Temp3 = 2.616979*220.0*(Analog->AnalogIn3)/2048.0*3.0 / (5.0-(Analog->AnalogIn3)/2048.0*3.0) - 260.1696;
	Analog->Temp4 = 2.616979*220.0*(Analog->AnalogIn4)/2048.0*3.0 / (5.0-(Analog->AnalogIn4)/2048.0*3.0) - 260.1696;
	
	float V = (Analog->AnalogIn1)/2048.0*3.3;
	Analog->R1 = (V*221)/(5-V);
	V = (Analog->AnalogIn1)/2048.0*3.3;
	Analog->R2 = (V*221)/(5-V);
	V = (Analog->AnalogIn1)/2048.0*3.3;
	Analog->R3 = (V*221)/(5-V);
	V = (Analog->AnalogIn1)/2048.0*3.3;
	Analog->R4 = (V*221)/(5-V);
}

uint8_t FindNextFilename(char * filename){
	FILINFO fno;
	uint8_t i = 0;
	char name[8];
	do{
		i++;
		sprintf(name, "%03u.csv",i);
	}while(f_stat(name, &fno) == FR_OK);
	*filename++ = name[0];
	*filename++ = name[1];
	*filename++ = name[2];
	*filename++ = name[3];
	*filename++ = name[4];
	*filename++ = name[5];
	*filename++ = name[6];
	*filename++ = name[7];
	*filename++ = name[8];
	
	return i;
}

uint16_t FindTableLength(char * table){
	uint16_t i=0;
	while((*table++) && (i < 512)) i++;
	return i;
}
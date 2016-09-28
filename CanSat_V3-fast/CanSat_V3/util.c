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
	//----------------packet count-----------------------
	tmp = allData->RTC->frameTeleCount;
	allData->frame_b->frameASCII[i++] = (tmp/10000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/1000)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/100)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp/10)%10 + 48;
	allData->frame_b->frameASCII[i++] = (tmp)%10 + 48;
	allData->frame_b->frameASCII[i++] = ',';
	//------------ state-------------------------
	allData->frame_b->frameASCII[i++] = 'S';
	allData->frame_b->frameASCII[i++] = allData->stan->armed_trigger+48;
	allData->frame_b->frameASCII[i++] = allData->stan->telemetry_trigger+48;
	allData->frame_b->frameASCII[i++] = allData->stan->flash_trigger+48;
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
	
	allData->frame_b->frameASCII[i++] = '\r';
	allData->frame_b->frameASCII[i++] = '\n';		//248 bajtów
	allData->frame_b->frameASCII[i++] = 0;
	allData->frame_b->frameASCII[i++] = 'X';
}

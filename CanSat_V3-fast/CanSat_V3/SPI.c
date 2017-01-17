/*
* SPI.c
*
* Created: 2015-05-15 11:45:16
*  Author: stratus
*/
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "struct.h"
#include "SPI.h"
#include "CanSat.h"

void SPI_W_Byte(uint8_t byte){
	ADC_SPI.DATA = byte;
	while(!(ADC_SPI.STATUS & SPI_IF_bm)){}
	volatile char tmp = ADC_SPI.DATA;
}

uint8_t SPI_R_Byte(void){
	ADC_SPI.DATA = 0x00;
	while(!(ADC_SPI.STATUS & SPI_IF_bm)){}
	return ADC_SPI.DATA;
}

//=======================FLASH===============================
void FLASH_CS(uint8_t CS) { 
	if(CS) FLASH_PORT.OUTCLR = FLASH_CS_pin; //Aktywuj pamiêæ Flash
	else FLASH_PORT.OUTSET = FLASH_CS_pin;
}


void FLASH_SetMaxBaudrate() {
	FLASH_SPI.CTRL = (FLASH_SPI.CTRL & (~SPI_PRESCALER_gm)) | SPI_PRESCALER_DIV4_gc | SPI_CLK2X_bm;
}

uint8_t FLASH_RW_Byte(uint8_t byte) {
	FLASH_SPI.DATA = byte;
	while(!(FLASH_SPI.STATUS & SPI_IF_bm));
	FLASH_SPI.STATUS = SPI_IF_bm;
	return FLASH_SPI.DATA;
}

uint16_t FLASH_ReadID(void){
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(0x90);
	FLASH_RW_Byte(0x00);
	FLASH_RW_Byte(0x00);
	FLASH_RW_Byte(0x00);
	uint16_t tmp = FLASH_RW_Byte(0) << 8;
	tmp |= FLASH_RW_Byte(0);
	FLASH_CS(0);
	return tmp;
}

void FLASH_1byteCommand(uint8_t value){
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(value);
	FLASH_CS(0);
}
void FLASH_2byteCommand(uint16_t value){
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(value>>8);
	FLASH_RW_Byte(value&0xFF);
	FLASH_CS(0);
}

void FLASH_WriteEnable(uint8_t value){
	if(value) FLASH_1byteCommand(0x06);
	else FLASH_1byteCommand(0x04);
}

uint8_t FLASH_ReadByte(uint32_t address){
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(0x03);	//READ
	//FLASH_RW_Byte(address>>3);
	FLASH_RW_Byte((address>>16) & 0xFF);
	FLASH_RW_Byte((address>>8) & 0xFF);
	FLASH_RW_Byte(address & 0xFF);
	uint16_t tmp = FLASH_RW_Byte(0);
	FLASH_CS(0);
	return tmp;
}
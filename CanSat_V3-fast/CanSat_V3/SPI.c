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
	if(CS) FLASH_PORT.OUTCLR = FLASH_CS_pin; //Aktywuj pami�� Flash
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
	FLASH_waitForReady();
	FLASH_CS(1);
	FLASH_RW_Byte(READ4);	//READ
	FLASH_RW_Byte((address>>24) & 0xFF);
	FLASH_RW_Byte((address>>16) & 0xFF);
	FLASH_RW_Byte((address>>8) & 0xFF);
	FLASH_RW_Byte(address & 0xFF);
	uint8_t tmp = FLASH_RW_Byte(0);
	FLASH_CS(0);
	return tmp;
}

void FLASH_arrayRead(uint32_t address, uint8_t * array, uint32_t length){
	FLASH_waitForReady();
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(READ4);	//READ
	FLASH_RW_Byte((address>>24) & 0xFF);
	FLASH_RW_Byte((address>>16) & 0xFF);
	FLASH_RW_Byte((address>>8) & 0xFF);
	FLASH_RW_Byte(address & 0xFF);
	for (uint32_t i=0; i<length; i++){
		array[i] = FLASH_RW_Byte(0);
	}
	FLASH_CS(0);
}

void FLASH_pageRead(uint32_t page, uint8_t * array, uint16_t length){
	uint32_t address = page<<9;
	FLASH_waitForReady();
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(READ4);	//READ
	FLASH_RW_Byte((address>>24) & 0xFF);
	FLASH_RW_Byte((address>>16) & 0xFF);
	FLASH_RW_Byte((address>>8) & 0xFF);
	FLASH_RW_Byte(address & 0xFF);
	for (uint32_t i=0; i<length; i++){
		array[i] = FLASH_RW_Byte(0);
	}
	FLASH_CS(0);
}

void FLASH_chipErase(void){
	FLASH_waitForReady();
	FLASH_WriteEnable(1);

	FLASH_CS(1);
	FLASH_RW_Byte(CE);
	FLASH_CS(0);
	FLASH_waitForReady();
}

void FLASH_sectorErase(uint32_t address){
	
}


void FLASH_setup(void){
	
}

uint8_t FLASH_status(void){
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(RDSR);	//READ
	uint8_t stat = FLASH_RW_Byte(0);
	FLASH_CS(0);
	return stat;
}

void FLASH_waitForReady(void){
	while(FLASH_status() & 0x01) _delay_us(10);
}

void FLASH_byteWrite(uint32_t address, uint8_t value){
	FLASH_waitForReady();
	FLASH_WriteEnable(1);
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(PP4);	//Page program
	FLASH_RW_Byte((address>>24) & 0xFF);
	FLASH_RW_Byte((address>>16) & 0xFF);
	FLASH_RW_Byte((address>>8) & 0xFF);
	FLASH_RW_Byte(address & 0xFF);
	FLASH_RW_Byte(value);
	FLASH_CS(0);
}

void FLASH_pageWrite(uint32_t page, uint8_t * array, uint16_t length){
	uint32_t address = page<<9;
	FLASH_waitForReady();
	FLASH_WriteEnable(1);
	FLASH_SetMaxBaudrate();
	FLASH_CS(1);
	FLASH_RW_Byte(PP4);	//Page program
	FLASH_RW_Byte((address>>24) & 0xFF);
	FLASH_RW_Byte((address>>16) & 0xFF);
	FLASH_RW_Byte((address>>8) & 0xFF);
	FLASH_RW_Byte(address & 0xFF);
	for(uint16_t i = 0; i < length; i++){
		FLASH_RW_Byte(*array++);
	}
	FLASH_CS(0);
}
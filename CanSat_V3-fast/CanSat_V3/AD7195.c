/*
 * AD7195.c
 *
 * Created: 2016-10-01 00:06:31
 *  Author: baretk
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "AD7195.h"
#include "struct.h"
#include <util/delay.h>

void AD7195_CS(uint8_t chipNo, bool state){
	if((chipNo == 0) && (state == false))  PORTC.OUTSET = PIN3_bm;
	if((chipNo == 0) && (state == true)) PORTC.OUTCLR = PIN3_bm;
	if((chipNo == 1) && (state == false))  PORTC.OUTSET = PIN4_bm;
	if((chipNo == 1) && (state == true)) PORTC.OUTCLR = PIN4_bm;
}

void AD7195_regwrite(uint8_t chipNo, uint8_t address, uint32_t value){
	AD7195_CS(chipNo, true);
	SPI_W_Byte(address);					//Select target register
	SPI_W_Byte((value>>16) & 0x0000FF);		//send oldest byte
	SPI_W_Byte((value>>8)  & 0x0000FF);		
	SPI_W_Byte( value      & 0x0000FF);		
	AD7195_CS(chipNo, true);
}

void AD7195_Init(uint8_t chipNo){
	
	// Ustawienie Config register
	AD7195_CS(chipNo, true);
	SPI_W_Byte(0b00010000);					//Write, config register, no cont. reading ??
	SPI_W_Byte(0b00000000);					//CHOP off, AC ex off
	SPI_W_Byte(0b11110000);					//przemiatanie kana³ów 1-4
	SPI_W_Byte(0b00011111);					//current source off, refdet off, buffer on, unipolar, 128gain
	AD7195_CS(chipNo, true);
	_delay_ms(10);
	// Ustawienie Mode register
	AD7195_CS(chipNo, true);
	SPI_W_Byte(0b00001000);					//Write, mode register
	SPI_W_Byte(0b00010100);					//conti conv, status reg read with data, ext clock
	SPI_W_Byte(0b00000000);					//sinc4, no parity check, no one cycle, 
	SPI_W_Byte(0b00000001);					//FS=1 -> 4.8 kSps
	AD7195_CS(chipNo, true);
}

uint8_t AD7195_WhoIam(uint8_t ChipNo){
	AD7195_CS(ChipNo, true);
	SPI_W_Byte(0b01100000);					//Read, ID register
	char id = SPI_R_Byte();					//CHOP off, AC ex off
	AD7195_CS(ChipNo, true);
	
	return id;
}

void AD7195_Sync(void){
	PORTD.OUTCLR = PIN1_bm;						//konfiguracja stanu pinu buzzer
	_delay_ms(100);
	PORTD.OUTSET = PIN1_bm;						//konfiguracja stanu pinu buzzer
}

void AD7195_Reset(uint8_t chipNo){
	AD7195_CS(chipNo, true);
	SPI_W_Byte(0x01);					
	SPI_W_Byte(0xFF);					
	SPI_W_Byte(0xFF);					
	SPI_W_Byte(0xFF);					
	SPI_W_Byte(0xFF);					
	SPI_W_Byte(0xFF);					
	SPI_W_Byte(0xFF);					
	AD7195_CS(chipNo, true);
}

void AD7195_ContConvRead(uint8_t * channel1, uint8_t * channel2, uint32_t * value1, uint32_t * value2){
	AD7195_CS(0, true);
	SPI_W_Byte(0x58);
	(*value1) = (uint32_t)SPI_R_Byte() << 16;
	(*value1) |= (uint32_t)SPI_R_Byte() << 8;
	(*value1) |= (uint32_t)SPI_R_Byte();
	(*channel1) = SPI_R_Byte() & 0x0F;
	AD7195_CS(0, false);
	
	AD7195_CS(1, true);
	SPI_W_Byte(0x58);
	(*value2) = (uint32_t)SPI_R_Byte() << 16;
	(*value2) |= (uint32_t)SPI_R_Byte() << 8;
	(*value2) |= (uint32_t)SPI_R_Byte();
	(*channel2) = SPI_R_Byte() & 0x0F;
	AD7195_CS(1, false);
}

void AD7195_ContRead(uint8_t chipNo, bool enable)
{
	AD7195_CS(chipNo, true);
	if(enable) SPI_W_Byte(0x5C);
	else SPI_W_Byte(0x5C);
	AD7195_CS(chipNo, false);
}

bool AD7195_RDY(uint8_t chipNo){
	AD7195_CS(chipNo, true);
	SPI_W_Byte(0b01000000);					//Read, ID register
	char status = SPI_R_Byte();					//CHOP off, AC ex off
	AD7195_CS(chipNo, true);
	
	return status & 0x80;
}

void AD7195_ReadStore(allData_t * allData){
	uint8_t channel1, channel2;
	uint32_t value1, value2;
	AD7195_ContConvRead(&channel1, &channel2, &value1, &value2);
	switch(channel1){
		case 4: allData->AD7195->raw_press1 = value1; break;
		case 5: allData->AD7195->raw_press2 = value1; break;
		case 6: allData->AD7195->raw_press3 = value1; break;
		case 7: allData->AD7195->raw_press4 = value1; break;
	}
	switch(channel2){
		case 4: allData->AD7195->raw_press5 = value2; break;
		case 5: allData->AD7195->raw_press6 = value2; break;
		case 6: allData->AD7195->raw_press7 = value2; break;
		case 7: allData->AD7195->raw_press8 = value2; break;
	}
}

void AD7195_PressureCalc(AD7195_t * AD7195){
	AD7195->pressure1 = AD7195->raw_press1/1677721.0 - 0;
	AD7195->pressure2 = AD7195->raw_press2/1677721.0 - 0;
	AD7195->pressure3 = AD7195->raw_press3/1677721.0 - 0;
	AD7195->pressure4 = AD7195->raw_press4/1677721.0 - 0;
	AD7195->pressure5 = AD7195->raw_press5/1677721.0 - 0;
	AD7195->pressure6 = AD7195->raw_press6/1677721.0 - 0;
	AD7195->pressure7 = (AD7195->raw_press7 - 16675031.0)/433.44;
	AD7195->pressure8 = (AD7195->raw_press8 - 16676277.0)/570.0;
}
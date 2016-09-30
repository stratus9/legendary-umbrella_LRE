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

void AD7195_regwrite(uint8_t chipNo, uint8_t address, uint8_t value){
	AD7195_CS(0, true);
	SPI_W_Byte(0b00010000);					//Write, config register, no cont. reading ??
	SPI_W_Byte(0b00000000);					//CHOP off, AC ex off
	SPI_W_Byte(0b11110000);					//przemiatanie kana³ów 1-4
	SPI_W_Byte(0b00011111);					//current source off, refdet off, buffer on, unipolar, 128gain
	AD7195_CS(0, true);
}
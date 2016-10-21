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


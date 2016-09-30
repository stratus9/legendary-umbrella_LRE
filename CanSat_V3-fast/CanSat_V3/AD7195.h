/*
 * AD7195.h
 *
 * Created: 2016-10-01 00:06:44
 *  Author: baretk
 */ 


#ifndef AD7195_H_
#define AD7195_H_

#include "SPI.h"
void AD7195_CS(uint8_t chipNo, bool state);
void AD7195_regwrite(uint8_t chipNo, uint8_t address, uint8_t value);
#endif /* AD7195_H_ */
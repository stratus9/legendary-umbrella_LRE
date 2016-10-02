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
void AD7195_regwrite(uint8_t chipNo, uint8_t address, uint32_t value);
void AD7195_Init(uint8_t chipNo);
uint8_t AD7195_WhoIam(uint8_t ChipNo);
void AD7195_Sync(void);
void AD7195_Reset(uint8_t chipNo);
void AD7195_ContConvRead(uint8_t * channel1, uint8_t * channel2, uint32_t * value1, uint32_t * value2);
void AD7195_ContRead(uint8_t chipNo, bool enable);
bool AD7195_RDY(uint8_t chipNo);
void AD7195_ReadStore(allData_t * allData);
void AD7195_PressureCalc(AD7195_t * AD7195);
#endif /* AD7195_H_ */
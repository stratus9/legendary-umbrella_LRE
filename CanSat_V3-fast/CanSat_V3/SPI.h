/*
 * SPI.h
 *
 * Created: 2015-05-15 11:45:41
 *  Author: stratus
 */ 


#ifndef SPI_H_
#define SPI_H_
#include <stdbool.h>
#include "struct.h"

#define FLASH_CS_pin   PIN4_bm       //Sygna³ CS karty SD
#define FLASH_PORT PORTE        //Port przez który pod³¹czona jest karta SD
#define FLASH_SPI  SPIE         //Alternatywnie port SPI do komunikacji

void SPI_W_Byte(uint8_t byte);
uint8_t SPI_R_Byte(void);

void FLASH_CS(uint8_t CS);
void FLASH_SetMaxBaudrate();
uint8_t FLASH_RW_Byte(uint8_t byte);
uint16_t FLASH_ReadID(void);
void FLASH_1byteCommand(uint8_t value);
void FLASH_2byteCommand(uint16_t value);
void FLASH_WriteEnable(uint8_t value);
uint8_t FLASH_ReadByte(uint32_t address);
#endif /* SPI_H_ */

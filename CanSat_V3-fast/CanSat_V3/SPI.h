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

void SPI_W_Byte(uint8_t byte);
uint8_t SPI_R_Byte(void);

#endif /* SPI_H_ */

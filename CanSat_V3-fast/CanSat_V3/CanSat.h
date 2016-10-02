/*
 * CanSat.h
 *
 * Created: 2015-03-22 19:14:56
 *  Author: stratus
 */ 


#ifndef CANSAT_H_
#define CANSAT_H_

//===================Definicja parametrów pracy=================
#define CPU_clock		32							//czêstotliwoœæ taktowania procesora
#define sampling_rate	25.0						//czêstotliwoœæ próbkowania w Hz
#define sampling_time	1/sampling_rate*1000.0		//czas próbkowania w ms
#define telemetry_rate	1							//czêstotliwoœæ telemetrii
#define telemetry_time	1/telemetry_rate*1000.0		//czas telemetrii
#define BUZZER_ONOFF	1							//wy³¹cznik buzzera
#define STARTUP_tele	1							//telemetria wy³¹czona na starcie
#define STARTUP_flash	0							//zapis do pamiêci wy³¹czony na starcie
#define STARTUP_armed	0							//rozbrojony na starcie
#define calibrationCNT	(int)(ceil(2*sampling_rate))				//iloœæ próbek do kalibracji

//----------------------------------Data interfaces--------
#define FLASH_SPI	SPIC
#define SPI_PORT	PORTC
#define FLASH_CS_PORT PORTC
#define SPI_speed		2000	//prêdkoœæ SPI w kHz

#define	SENSORS_I2C	TWIC
#define I2C_speed		300		//prêdkoœæ interfejsu I2C w kHz


//----------------------------------Definicja ledów---------
//	LED1 - green	- sensor update
//	LED2 - red		- bad ISR
//	LED3 - blue		- receive XB, remote command
//	LED4 - orange	- GPS
//	LED5 - yellow	- calibration

#define LED1 PIN0_bm	//sequence error
#define LED2 PIN1_bm	//Sequence halt
#define LED3 PIN2_bm	//seq in progress
#define LED4 PIN3_bm	//out update
#define LED5 PIN4_bm	//measur
#define LED6 PIN5_bm	//memory access
#define LED_PORT PORTF

//----------------------------Definicja przycisków-------------------
//	SW1 -			- Read
//	SW2 -			- Bootloader

//#define SW1 
//#define SW2

//---------------------------Definicja parametrów filtrów exp--------------
#define LPS25H_alti_alpha 0.2
#define LPS25H_velo_alpha 0.1
#define MPU_acc_alpha 0.3
#define MPU_velo_alpha 0.3
#define MPU_alti_alpha 0.3
#define MPU_gyro_alpha 0.3
#define MPU_angle_alpha 0.3
#define BAT_voltage_alpha 0.05

//-------------------------Definicje parametrów filtrów komplementarnych----
#define altitude_acc_press_beta	0.1
#define velocity_acc_press_beta	0.1


void USART_SendCache(void);
uint8_t I2C_ReadEnd(bool koniec);
void structInit(void);
void Initialization(void);
void UpdateFW(void);
void SensorUpdate(allData_t *);
void FLASHerase(void);
#endif /* CANSAT_H_ */

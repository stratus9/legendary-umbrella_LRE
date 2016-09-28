/*
 * struct.h
 *
 * Created: 2015-03-22 16:41:01
 *  Author: stratus
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef STRUCT_H_
#define STRUCT_H_

//-------------------------------struktura obs³ugi maszyny stanów-------------------
typedef struct stan_s{
	bool new_data;
	bool new_frame;
	bool cmd_mode;
	bool telemetry_trigger;
	bool flash_trigger;
	bool armed_trigger;
	uint8_t flightState;
	uint8_t softwareState;
} stan_t;

typedef struct Output_s{
	bool relay1;
	bool relay2;
	bool relay3;
	bool relay4;
	bool relay5;
	bool relay6;
	bool relay7;
	bool servo1;
	bool servo2;
	} Output_t;

//-------------------------------struktura obs³ugi interfejsu USART----------------
typedef struct USART_s{
	uint8_t in_i;			//licznik pozycji bufora wejœciowego
	char in[50];			//bufer wejœciowy
	bool in_inprogress;		//trwa odbiór
	bool in_ready;			//odebrano ramkê
	bool in_error;			//b³¹d odbioru
	
	uint8_t out_i;			//licznik pozycji bufora wejœciowego
	char out[50];			//bufer wejœciowy
	bool out_inprogress;	//trwa odbiór
	bool out_ready;			//odebrano ramkê
	bool out_error;			//b³¹d odbioru
	
	bool TxFlag;			//sterowanie transmisj¹
	bool RxFlag;
} USARTdata_t;

//---------------------------struktura obs³ugi ADC--------------------------------
typedef struct ADC_s{
	uint16_t ADC16u;
	int16_t ADC16s;
	float AnalogIn1;
	float AnalogIn2;
	float AnalogIn3;
	float AnalogIn4;
	float AnalogIn5;
	float AnalogIn6;
	float AnalogIn7;
	float AnalogIn8;
	float Press1;
	float Press2;
	float Press3;
	float Press4;
	float Press5;
	float Press6;
	float Press7;
	float Press8;
	} Analog_t;

//------------------------FRAME--------------------------------------------------
typedef struct frame_s{
	char frameASCII[600];
	uint16_t iUART;
	bool mutex;
	bool terminate;
	float max_acc;
	//GPS
	char latitude[14];
	char longitude[14];
	char altitude[9];
} frame_t;

typedef struct buzzer_s{
	bool trigger;
	uint8_t mode;
	uint8_t i;
	uint8_t count;
} buzzer_t;

typedef struct RTC_s{
	uint32_t time;				//jest w sekundach -> zmieni? na ms
	uint32_t frameTeleCount;
	uint32_t frameFlashCount;
} RTC_t;
		
typedef struct allData_s{
	Analog_t * Analog;
	stan_t * stan;
	frame_t * frame;
	frame_t * frame_b;
	RTC_t * RTC;
	Output_t * Output;
} allData_t;

#endif /* STRUCT_H_ */
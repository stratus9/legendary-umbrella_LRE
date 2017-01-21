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
	bool run_trigger;
	bool Abort;
	uint8_t State;
	uint32_t IgnTime;
	uint32_t FireTime;
	bool MFV;
	bool MOV;
	bool FPV;
	bool MPV;
	bool IGN;
	bool SERVO1;
	bool SERVO2;
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
	int16_t AnalogIn1;
	int16_t AnalogIn2;
	int16_t AnalogIn3;
	int16_t AnalogIn4;
	int16_t AnalogIn5;
	int16_t AnalogIn6;
	int16_t AnalogIn7;
	int16_t AnalogIn8;
	uint16_t Temp1;
	uint16_t Temp2;
	uint16_t Temp3;
	uint16_t Temp4;
	float R1;
	float R2;
	float R3;
	float R4;
	} Analog_t;

//------------------------FRAME--------------------------------------------------
typedef struct frame_s{
	char frameASCII[200];
	char frameBIN[200];
	uint16_t iUART;
	bool mutex;
	bool terminate;
} frame_t;

typedef struct frameSD_s{
	char frameASCII[530];
} frameSD_t;

typedef struct buzzer_s{
	bool trigger;
	uint8_t mode;
	uint8_t i;
	uint8_t count;
} buzzer_t;

typedef struct Clock_s{
	uint32_t time;				//jest w 10 ms
	uint32_t RealTime;				//jest w 2 us
	uint32_t frameTeleCount;
	uint32_t frameFlashCount;
} Clock_t;

typedef struct AD7195_s{
	float pressure1;
	float pressure2;
	float pressure3;
	float pressure4;
	float pressure5;
	float pressure6;
	float pressure7;
	float pressure8;
	
	uint32_t raw_press1;
	uint32_t raw_press2;
	uint32_t raw_press3;
	uint32_t raw_press4;
	uint32_t raw_press5;
	uint32_t raw_press6;
	uint32_t raw_press7;
	uint32_t raw_press8;
	int32_t raw_tenso1;
	int32_t raw_tenso2;
} AD7195_t;
		
typedef struct allData_s{
	Analog_t * Analog;
	stan_t * stan;
	frame_t * frame;
	frame_t * frame_b;
	Clock_t * Clock;
	Output_t * Output;
	AD7195_t * AD7195;
} allData_t;

typedef union float2array_u{
	float floatNumber;
	char arrayNumber[4];
} float2array_t;

typedef union int32_2array_u{
	uint32_t uintNumber;
	int32_t intNumber;
	char arrayNumber[4];
} int32_2array_t;

typedef union int16_2array_u{
	uint16_t uintNumber;
	int16_t intNumber;
	char arrayNumber[2];
} int16_2array_t;

typedef union {
	uint8_t array[64];
	struct{
		uint32_t press1;
		uint32_t press2;
		uint32_t press3;
		uint32_t press4;
		uint32_t press5;
		uint32_t press6;
		uint32_t press7;
		uint32_t press8;
		uint16_t temp1;
		uint16_t temp2;
		uint16_t temp3;
		uint16_t temp4;
		uint8_t IGN : 1;
		uint8_t MFV : 1;
		uint8_t MOV : 1;
		uint8_t WPV : 1;
		uint8_t FPV : 1;
		uint8_t servo1 :1;
		uint8_t servo2 :1;
		uint8_t redu :1;
		uint8_t : 0;	//wyrównanie
		uint32_t Clock;
		
		};
} FLASH_dataStruct_t;

typedef struct{
	uint8_t position;
	union{
		FLASH_dataStruct_t FLASH_dataStruct[8];
		uint8_t data[512];
		};
}FLASH_pageStruct_t;


#endif /* STRUCT_H_ */
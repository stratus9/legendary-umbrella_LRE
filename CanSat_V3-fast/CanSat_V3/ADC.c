/*
 * ADC.c
 *
 * Created: 2015-10-25 10:40:50
 *  Author: baretk
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "struct.h"
#include "ADC.h"
#include "CanSat.h"

void AnalogUpdate(Analog_t * Analog){
	float Analog1, Analog2, Analog3, Analog4, Analog5, Analog6, Analog7, Analog8;
	
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;	//singleended
	//sensor 1
	//ADCA_CH0_CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_1X_gc;
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN13_gc | ADC_CH_MUXNEG_GND_MODE4_gc;
	ADCA.CH0.CTRL |= ADC_CH_START_bm;				//rozpocznij pomiar CH0
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;	
	Analog1 = ADCA.CH0RES/16.0;
	
	//sensor 2
	//ADCA_CH0_CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_1X_gc;
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN14_gc | ADC_CH_MUXNEG_GND_MODE4_gc;
	ADCA.CH0.CTRL |= ADC_CH_START_bm;				//rozpocznij pomiar CH0
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
	Analog2 = ADCA.CH0RES/16.0;
	
	//sensor 3
	//ADCA_CH0_CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_1X_gc;
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN15_gc | ADC_CH_MUXNEG_GND_MODE4_gc;
	ADCA.CH0.CTRL |= ADC_CH_START_bm;				//rozpocznij pomiar CH0
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
	Analog3 = ADCA.CH0RES/16.0;
	
	//Vbat
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN7_gc;	    // 
	ADCA.CH0.CTRL |= ADC_CH_START_bm;				//rozpocznij pomiar CH0
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
	Analog4 = (ADCA.CH0RES/4096.0-0.05)*10.3;
	
	//Vex
	//ADCA_CH0_CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_1X_gc;
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN1_gc | ADC_CH_MUXNEG_GND_MODE4_gc;
	ADCA.CH0.CTRL |= ADC_CH_START_bm;				//rozpocznij pomiar CH0
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
	Analog5 = (ADCA.CH0RES/4096.0-0.05)*10.3;
	
	//Vusb
	//ADCA_CH0_CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_1X_gc;
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN8_gc | ADC_CH_MUXNEG_GND_MODE4_gc;
	ADCA.CH0.CTRL |= ADC_CH_START_bm;				//rozpocznij pomiar CH0
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
	Analog6 = (ADCA.CH0RES/4096.0-0.05)*10.3;
	
	//Vcc
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_INTERNAL_gc;	//internal
	ADCA.CH0.MUXCTRL = ADC_CH_MUXINT_SCALEDVCC_gc;
	ADCA.CH0.CTRL |= ADC_CH_START_bm;				//rozpocznij pomiar 1/10 VCC
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	ADCA.CH0.INTFLAGS = ADC_CH0IF_bm;
	Analog7 = ((ADCA.CH0RES/4096.0)-0.05)*10.35;
	
	Analog8 = 0;
	
	//-----Analog to percent-----------------------
	Analog->AnalogIn1 = ((Analog1 * 100) / 255);
	Analog->AnalogIn2 = ((Analog2 * 100) / 255);
	Analog->AnalogIn3 = ((Analog3 * 100) / 255);
	Analog->AnalogIn4 = ((Analog4 * 100) / 255);
	Analog->AnalogIn5 = ((Analog5 * 100) / 255);
	Analog->AnalogIn6 = ((Analog6 * 100) / 255);
	Analog->AnalogIn7 = ((Analog7 * 100) / 255);
	Analog->AnalogIn8 = ((Analog8 * 100) / 255);
}

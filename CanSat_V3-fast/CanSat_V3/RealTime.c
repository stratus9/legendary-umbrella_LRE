/*
 * RealTime.c
 *
 * Created: 2016-11-25 20:03:08
 *  Author: baretk
 */

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "RealTime.h"

void resetRTC(void) {
}

void setRTC(uint32_t value) {
}

uint16_t getRTC_ms(void) {
    return 0;
}

uint32_t getRTC_us(void) {
//	EVSYS_STROBE=(1<<1);				//Wygeneruj zdarzenie w kanale 1
	return (uint32_t)TCD1_CNT;			//Zwróæ wartoœæ CNT
}

void initRTC(void) {
	//------------------ Okres timera wynosi 1.19 h z rozdzielczoœci¹ 2us ---------------------------------------
	EVSYS_CH0MUX = EVSYS_CHMUX_TCC1_OVF_gc;
	EVSYS_CH1MUX = EVSYS_CHMUX_OFF_gc;								//¿adne zdarzenie nie jest zwi¹zane z kana³em 1
	TCD1.CTRLA = TC_CLKSEL_EVCH0_gc;								//wybór Ÿród³a taktowania
	TCD1.CTRLB = TC_WGMODE_NORMAL_gc | TC1_CCAEN_bm;				//tryb normalny i w³¹czenie przechwytywania kana³u A timera
	TCD1.CTRLD = TC_EVACT_CAPT_gc | TC_EVSEL_CH1_gc;				//OpóŸnij zdarzenie o 1 takt CLKper, przechwytywanie
	TCD1.CNT = 0;
	TCD1.PER = 0xFFFF;												//ograniczenie zakresu do 2^16 -> 6.5536s
	
	TCC1.CNT = 0;
	TCC1.PER = 3200;												//10kHz -> PER=3200, PRE=1@32MHz
	TCC1.CTRLB=TC_WGMODE_NORMAL_gc;									//tryb normalny
	TCC1.CTRLA=TC_CLKSEL_DIV1_gc;									//wybór Ÿród³a taktowania
}

void waitRTC_ms(uint16_t value) {
}


uint8_t checkRTC_ms(uint16_t value) {
	return 0;
}

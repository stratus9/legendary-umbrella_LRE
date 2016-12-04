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
	EVSYS_STROBE=(1<<1);						//Wygeneruj zdarzenie w kanale 1
	//return ((uint32_t)TCC1_CCA)<<16 | TCC0_CCA; //Zwróæ wartoœæ CNT
	return (uint32_t)TCC1.CCA * 100000 + 2*TCC0.CCA;
}

void initRTC(void) {
	//------------------ Okres timera wynosi 1.19 h z rozdzielczoœci¹ 2us ---------------------------------------
	EVSYS.CH0MUX=EVSYS_CHMUX_TCC0_OVF_gc;
	EVSYS.CH1MUX=EVSYS_CHMUX_OFF_gc;								//¿adne zdarzenie nie jest zwi¹zane z kana³em 1
	
	TCC1.CNT = 0;
	TCC1.PER = 0xFFFF;
	TCC1.CTRLA=TC_CLKSEL_EVCH0_gc;									//wybór Ÿród³a taktowania
	TCC1.CTRLB=TC_WGMODE_NORMAL_gc | TC1_CCAEN_bm;					//tryb normalny i w³¹czenie przechwytywania kana³u A timera
	TCC1.CTRLD=TC1_EVDLY_bm | TC_EVACT_CAPT_gc | TC_EVSEL_CH1_gc;	//OpóŸnij zdarzenie o 1 takt CLKper, przechwytywanie					
	
	TCC0.CNT = 0;	
	TCC0.PER = 50000;
	TCC0.CTRLB=TC_WGMODE_NORMAL_gc | TC0_CCAEN_bm;					//tryb normalny i w³¹czenie przechwytywania kana³u A timera
	TCC0.CTRLD=TC_EVACT_CAPT_gc | TC_EVSEL_CH1_gc;					//przechwytywanie
	TCC0.CTRLA=TC_CLKSEL_DIV64_gc;									//wybór Ÿród³a taktowania DIV64=>2us
}

void waitRTC_ms(uint16_t value) {
}


uint8_t checkRTC_ms(uint16_t value) {
	return 0;
}

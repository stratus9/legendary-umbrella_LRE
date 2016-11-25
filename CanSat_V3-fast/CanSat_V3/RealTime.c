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
	EVSYS_STROBE=(1<<1);    //Wygeneruj zdarzenie w kanale 1
	return (uint32_t)TCD1_CCA<<16 | TCC1_CCA; //Zwróæ wartoœæ CNT
}

void initRTC(void) {
	EVSYS_CH0MUX=EVSYS_CHMUX_TCC1_OVF_gc;
	EVSYS_CH1MUX=EVSYS_CHMUX_OFF_gc; //¿adne zdarzenie nie jest zwi¹zane z kana³em 1
	TCD1.CTRLA=TC_CLKSEL_EVCH0_gc;
	TCD1.CTRLB=TC_WGMODE_NORMAL_gc | TC1_CCAEN_bm;
	TCD1.CTRLD=TC0_EVDLY_bm | TC_EVACT_CAPT_gc | TC_EVSEL_CH1_gc;       //OpóŸnij zdarzenie o 1 takt CLKper, przechwytywanie
	
	TCC1.CTRLB=TC_WGMODE_NORMAL_gc | TC0_CCAEN_bm;
	TCC1.CTRLD=TC_EVACT_CAPT_gc | TC_EVSEL_CH1_gc;
	TCC1.CTRLA=TC_CLKSEL_DIV2_gc;
}

void waitRTC_ms(uint16_t value) {
}


uint8_t checkRTC_ms(uint16_t value) {
	return 0;
}

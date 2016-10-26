/*
 * Initialization.c
 *
 * Created: 2015-04-26 20:54:17
 *  Author: stratus
 */

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct.h"
#include "Initialization.h"
#include "CanSat.h"
#include "util.h"

void CPU_clk(uint8_t val) {
    Osc2MHz((uint8_t)(val / 2));
}

void Osc2MHz(uint8_t pll_factor) {
    OSC.CTRL |= OSC_RC2MEN_bm;						// w³¹czenie oscylatora 2MHz
    //--------------stabilizacja czêstotliwoœci
    DFLLRC2M.CALA = 0x40;								//dane z sygnatury
    DFLLRC2M.CALB = 0x0D;
    OSC.CTRL |= OSC_RC32KEN_bm;							//w³¹czenie oscylatora 32kHz
    while(!(OSC.STATUS & OSC_RC32KRDY_bm));				//czekanie na ustabilizowanie zegara 32kHz
    OSC.DFLLCTRL &= ~(OSC_RC32MCREF_gm | OSC_RC2MCREF_bm);
    DFLLRC2M.CTRL |= DFLL_ENABLE_bm;
    while(!(OSC.STATUS & OSC_RC2MRDY_bm));			// czekanie na ustabilizowanie zegara 2MHz
    CPU_CCP = CCP_IOREG_gc;							// odblokowanie zmiany Ÿród³a sygna³u
    CLK.CTRL = CLK_SCLKSEL_RC2M_gc;					// zmiana Ÿród³a sygna³u na RC 2MHz
    OSC.CTRL &= ~OSC_PLLEN_bm;						// wy³¹czenie PLL
    OSC.PLLCTRL = OSC_PLLSRC_RC2M_gc | pll_factor;	// mno¿nik czêstotliwoœci (od 1 do 31)
    OSC.CTRL = OSC_PLLEN_bm;						// w³¹czenie uk³adu PLL
    while(!(OSC.STATUS & OSC_PLLRDY_bm));			// czekanie na ustabilizowanie siê generatora PLL
    CPU_CCP = CCP_IOREG_gc;							// odblokowanie zmiany Ÿród³a sygna³u
    CLK.CTRL = CLK_SCLKSEL_PLL_gc;					// wybór Ÿród³a sygna³u zegarowego PLL
}

void OscRTC(void) {
    /*
    OSC.CTRL |= OSC_RC32KEN_bm;						//w³¹czenie oscylatora 32.768kHz
    while(!(OSC.STATUS & OSC_RC32KRDY_bm));			//czekanie na ustabilizowanie siê generatora
    CLK.RTCCTRL = CLK_RTCSRC_RCOSC32_gc;			//RTC zasilane z 32.768kHz
    CLK.RTCCTRL |= CLK_RTCEN_bm;					//w³¹czenie RTC
    */
}

void RTC_Init(void) {
    /*
    RTC.PER = 32768;								//okres 1s
    RTC.CNT = 0;									//zerowanie licznika
    RTC.CTRL = RTC_PRESCALER_DIV1_gc;				//preskaler 1 i w³¹czenie RTC
    RTC.INTCTRL = RTC_OVFINTLVL_MED_gc;				//przerwanie od przepe³nienia - niski priorytet
    */
}

void TimerCInit(uint16_t period_ms) {
    TCC0_CTRLA = TC_CLKSEL_DIV1024_gc;				//w³¹czenie timera z preskalerem 64
    TCC0_CTRLB = 0x00;								//Normal mode
    TCC0_CNT = 0;									//zeruj licznik
    TCC0_PER = (period_ms * 32000UL) / 1024;			//przeliczenie okresu na 1ms*period
    TCC0_INTCTRLA = TC_OVFINTLVL_MED_gc;			//œredni poziom przerwania od przepe³nienia
}

void TimerDInit(uint16_t period_ms) {
    TCD0_CTRLA = TC_CLKSEL_DIV1024_gc;				//w³¹czenie timera z preskalerem 64
    TCD0_CTRLB = 0x00;								//Normal mode
    TCD0_CNT = 0;									//zeruj licznik
    TCD0_PER = (period_ms * 32000UL) / 1024;			//przeliczenie okresu na 1ms*period
    TCD0_INTCTRLA = TC_OVFINTLVL_LO_gc;				//œredni poziom przerwania od przepe³nienia
}

void TimerEInit(uint16_t period_ms) {
    TCE0_CTRLA = TC_CLKSEL_DIV1024_gc;				//w³¹czenie timera z preskalerem 64
    TCE0_CTRLB = 0x00;								//Normal mode
    TCE0_CNT = 0;									//zeruj licznik
    TCE0_PER = (period_ms * 32000UL) / 1024;			//przeliczenie okresu na 1ms*period
    TCE0_INTCTRLA = TC_OVFINTLVL_MED_gc;			//œredni poziom przerwania od przepe³nienia
}

void TimerFInit(uint16_t period_ms) {
    TCF0_CTRLA = TC_CLKSEL_DIV1024_gc;				//w³¹czenie timera z preskalerem 64
    TCF0_CTRLB = 0x00;								//Normal mode
    TCF0_CNT = 0;									//zeruj licznik
    if(period_ms > 2000) period_ms = 2000;			//ograniczenie ze wzglêdu na przepe³nienie
    TCF0_PER = (period_ms * 32000UL) / 1024;			//przeliczenie okresu na 1ms*period
    TCF0_INTCTRLA = TC_OVFINTLVL_MED_gc;			//œredni poziom przerwania od przepe³nienia
}

void ADC_Init(void) {
    ADCB.CTRLA = ADC_ENABLE_bm;
    ADCB.CTRLB = ADC_RESOLUTION_12BIT_gc;		//rozdzielczoœæ 12bit, CONVMODE=0
    ADCB.CTRLB |= ADC_FREERUN_bm;				//ci¹g³a konwersja
    ADCB.CTRLB |= ADC_CONMODE_bm;				//tryb ze znakiem
    ADCB.EVCTRL = ADC_SWEEP_0123_gc;			//przemiatanie kana³ów 0,1,2,3
    ADCB.REFCTRL = ADC_REFSEL_INTVCC2_gc;		//wybór napiêcia odniesienia VCC/2
    ADCB.PRESCALER = ADC_PRESCALER_DIV512_gc;	//preskaler 62.5kHz@32MHz -> ~15ksps@32MHz
    ADCB.CH0.CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_DIV2_gc;	//tryb ró¿nicowy ze wzmocnieniem 1/2
    ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc | ADC_CH_MUXNEG_GND_MODE4_gc;	//pocz¹tek przemiatania od PIN13
    ADCB.CH1.CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_DIV2_gc;	//tryb ró¿nicowy ze wzmocnieniem 1/2
    ADCB.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc | ADC_CH_MUXNEG_GND_MODE4_gc;	//pocz¹tek przemiatania od PIN14
    ADCB.CH2.CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_DIV2_gc;	//tryb ró¿nicowy ze wzmocnieniem 1/2
    ADCB.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc | ADC_CH_MUXNEG_GND_MODE4_gc;	//pocz¹tek przemiatania od PIN15
    ADCB.CH3.CTRL = ADC_CH_INPUTMODE_DIFFWGAIN_gc | ADC_CH_GAIN_DIV2_gc;	//tryb ró¿nicowy ze wzmocnieniem 1/2
    ADCB.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc | ADC_CH_MUXNEG_GND_MODE4_gc;	//pocz¹tek przemiatania od PIN15
    ADCB.CH3.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;	//przerwanie od zakoñczenia przetwarzania ostatniego kana³u
}

void USART_Init(void) {
    PORTD.OUTCLR = PIN2_bm;					//konfiguracja pinów Rx i Tx
    PORTD.OUTSET = PIN3_bm;
    PORTD.DIRSET = PIN3_bm;
    USARTD0.CTRLC = USART_CHSIZE_8BIT_gc;	//ramka: 8bitów, 1 bit stopu, brak bitu parzystoœci
    int16_t BSEL = 1047;					//konfiguracja prêdkoœci transmisji 115200
    int8_t BSCALE = 0b10100000;				//-6
    USARTD0.BAUDCTRLA = (uint8_t)(BSEL & 0x00FF);
    USARTD0.BAUDCTRLB = (uint8_t)(((BSEL >> 8) & 0x000F) | BSCALE) ;
    USARTD0.CTRLA |= USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_HI_gc;		//odblokowanie przerwañ nadajnika i odbiornika, niski priorytet
    USARTD0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;						//w³¹czenie nadajnika i odbiornika USART
}

void IO_Init(void) {
    //konfiguracja przycisków
    PORTB.DIRCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
    PORTB.PIN4CTRL = PORT_OPC_PULLUP_gc;		//w³¹czenie Pull-up przycisku UP
    PORTB.PIN5CTRL = PORT_OPC_PULLUP_gc;		//w³¹czenie Pull-up przycisku DOWN
    PORTB.PIN6CTRL = PORT_OPC_PULLUP_gc;		//w³¹czenie Pull-up przycisku LEFT
    PORTB.PIN7CTRL = PORT_OPC_PULLUP_gc;		//w³¹czenie Pull-up przycisku RIGHT
    //konfiguracja LED
    PORTF.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm;	//konfiguracja kierunku pinów LED
    PORTF.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm;	//konfiguracja stanu pinów LED
    //konfiguracja buzzera
    PORTF.DIRSET = PIN6_bm;						//konfiguracja kierunku pinu buzzera
    PORTF.OUTCLR = PIN6_bm;						//konfiguracja stanu pinu buzzer
    PORTD.DIRSET = PIN0_bm;						//konfiguracja kierunku pinu buzzera
    PORTD.OUTCLR = PIN0_bm;						//konfiguracja stanu pinu buzzer
    //konfiguracja wyjœæ RELAY
    PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;			//konfiguracja kierunku pinów RELAY
    PORTE.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm;			//konfiguracja stanu pinów RELAY
    PORTR.DIRSET = PIN0_bm | PIN1_bm;					//konfiguracja kierunku pinów RELAY
    PORTR.OUTCLR = PIN0_bm | PIN1_bm;					//konfiguracja stanu pinów RELAY
    PORTA.DIRSET = PIN6_bm | PIN7_bm;					//konfiguracja kierunku pinów RELAY
    PORTA.OUTCLR = PIN6_bm | PIN7_bm;					//konfiguracja stanu pinów RELAY
    //konfiguracja interfejsu XBEE
    PORTD_OUTCLR = PIN2_bm;		//RX
    PORTD_DIRSET = PIN3_bm;		//TX
    PORTD_DIRSET = PIN5_bm;		//sleep pin
    PORTD_OUTCLR = PIN5_bm;
    //konfiguracja pinów I2C
    PORTC_DIR = PIN0_bm | PIN1_bm;
    PORTC_OUT = PIN0_bm | PIN1_bm;
    //konfiguracja wejœæ ADC
    PORTA.OUTCLR = PIN0_bm | PIN1_bm;							//Vsense
    PORTB.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;		//Temperature
    //wyjœcia servo
    PORTD.DIRSET = PIN4_bm | PIN5_bm;					//konfiguracja kierunku pinów SERVO
    PORTD.OUTCLR = PIN4_bm | PIN5_bm;					//konfiguracja stanu pinów SERVO
    //ADC sync
    PORTD.DIRSET = PIN1_bm;						//konfiguracja kierunku pinu synchronizacja ADC
    PORTD.OUTSET = PIN1_bm;						//konfiguracja stanu pinu synchronizacji ADC
    //podœwietlenie LCD
    PORTC.DIRSET = PIN2_bm;
    PORTC.OUTCLR = PIN2_bm;
}

void I2C_Init(void) {
    SENSORS_I2C.MASTER.CTRLA = TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm;		//ustawienie priorytetu przerwania na Low i w?aczenie przerwania od odbioru
    SENSORS_I2C.MASTER.CTRLB = TWI_MASTER_SMEN_bm;                             //uruchomienie Smart Mode
    SENSORS_I2C.MASTER.BAUD = 48;                                             //BAUD = 48 -> f=300kHz    BAUD = 21 -> f=600kHz
    SENSORS_I2C.MASTER.CTRLA |= TWI_MASTER_ENABLE_bm;                          //w³¹czenie TWI
    SENSORS_I2C.MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;                   //I2C wolne
}

void SPI_Init(void) {
    SPI_PORT.DIRSET = PIN4_bm;						//podci¹gniêcie CS
    SPI_PORT.DIRSET = PIN5_bm | PIN7_bm;
    SPI_PORT.DIRCLR = PIN6_bm;
    SPI_PORT.OUTSET = PIN5_bm | PIN6_bm | PIN7_bm;	//mo¿e dodaæ PIN6_bm
    //-----------Clk = 4 MHz-------------------------
    ADC_SPI.CTRL = SPI_ENABLE_bm | SPI_MODE_0_gc | SPI_PRESCALER_DIV64_gc | SPI_CLK2X_bm | SPI_MASTER_bm;
    //FLASH_SPI.INTCTRL = SPI_INTLVL_LO_gc;
}

uint8_t ReadSignatureByte(uint16_t Address) {
    NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
    uint8_t Result;
    __asm__ ("lpm %0, Z\n" : "=r" (Result) : "z" (Address));
    NVM_CMD = NVM_CMD_NO_OPERATION_gc;
    return Result;
}

void Clock_Init(void){
	//Iniclajalizacja RTC itd
}
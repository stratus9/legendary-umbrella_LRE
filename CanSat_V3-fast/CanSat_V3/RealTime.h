/*
 * RealTime.h
 *
 * Created: 2016-11-25 20:03:17
 *  Author: baretk
 */ 


#ifndef REALTIME_H_
#define REALTIME_H_

void initRTC(void);
void resetRTC(void);
void setRTC(uint32_t);
uint16_t getRTC_ms(void);
uint32_t getRTC_us(void);
void waitRTC_ms(uint16_t);
uint8_t checkRTC_ms(uint16_t);





#endif /* REALTIME_H_ */

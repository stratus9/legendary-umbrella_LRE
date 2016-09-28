/*
 * util.h
 *
 * Created: 2015-03-22 20:24:23
 *  Author: stratus
 */ 

#ifndef UTIL_H_
#define UTIL_H_

void float2char(float, char *);
void prepareFrame(allData_t *);
void Ignition_active(void);
void Ignition_inactive(void);
void MFV_valve_open(void);
void MFV_valve_close(void);
void MOV_valve_open(void);
void MOV_valve_close(void);
void MPV_valve_open(void);
void MPV_valve_close(void);
void FPV_valve_open(void);
void FPV_valve_close(void);
void SERVO1_open(void);
void SERVO1_close(void);
void SERVO2_open(void);
void SERVO2_close(void);
void Buzzer_active(void);
void Buzzer_inactive(void);
void ADC_sync(void);
void BL_onoff(bool state);
#endif /* UTIL_H_ */

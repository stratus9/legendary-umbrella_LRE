/*
 * util.h
 *
 * Created: 2015-03-22 20:24:23
 *  Author: stratus
 */ 

#ifndef UTIL_H_
#define UTIL_H_

void float2char(float, char *);
void prepareFrameFlash(allData_t *);
void prepareFrame(allData_t *);
void prepareFrameBIN(allData_t * allData);
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
void SERVO_open(void);
void SERVO_close(void);
void Buzzer_active(void);
void Buzzer_inactive(void);
void Light_Green(void);
void Light_Red(void);
void BL_onoff(bool state);
void CheckOutputState(stan_t *);
uint16_t Add2Buffer(frame_t *, frameSD_t *);
void ADC_tempCalc(Analog_t *);
uint8_t FindNextFilename(char *);
uint16_t FindTableLength(char *);
void ClockReset(allData_t * allData);
void ClockGet(allData_t * allData);
#endif /* UTIL_H_ */

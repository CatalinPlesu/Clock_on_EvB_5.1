/*
 * Pwm.h
 *
 * Created: 8/22/2022 2:12:10 PM
 *  Author: catalin
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>
#include "PwmCfg.h"

typedef enum {
	PwmStateIdle,
	PwmStateRunning,
	PwmStateRunningSilent,
}
PwmState;

void PwmInit();
void PwmRutine();
void PwmToggle();
void PwmStart();
void PwmSetDuty(uint8_t dutycycle);


#endif /* PWM_H_ */
/*
 * SevSeg.h
 *
 * Created: 7/13/2022 2:43:21 PM
 *  Author: alexei.guriev
 */ 


#ifndef DISP7SEG_H_
#define DISP7SEG_H_

#include <stdint.h>

#include "StatuError.h"
#include "SevSegCfg.h"
#include "TimerSw.h"

void SevSegInit(void);

void SevSegRutine(void);

StatusError SevSegSetTimeVal(uint8_t digits[4], uint8_t dots);

StatusError SevSegSetTemperatureVal(int8_t value);

StatusError SevSegSetFloatVal(float value);

StatusError SevSegSetByDigit(uint8_t digitIndex, uint8_t digitValue, bool withComa);

StatusError SevSegSetByDigitCostum(uint8_t digitIndex, uint8_t digitValue);

#endif /* DISP7SEG_H_ */

/*
 * Adc.h
 *
 * Created: 8/2/2022 4:03:40 PM
 *  Author: User
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>
#include "AdcCfg.h"

typedef struct {
	uint16_t adcChannel[ADC_CHANNELS_USED];
}AdcValue;

void AdcInit(void);

const AdcValue* GetAdcValue(uint8_t channel);


#endif /* ADC_H_ */
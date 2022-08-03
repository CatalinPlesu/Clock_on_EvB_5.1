#ifndef ADCCFG_H_
#define ADCCFG_H_

#include <avr/io.h>

#define ADC_CHANNEL_1_PIN PA6
#define ADC_CHANNEL_2_PIN PA7
#define ADC_CHANNELS_USED 2

typedef struct
{
	uint8_t adcActiveChannel;
	uint8_t adcChannelsCount;
	uint8_t adcPinValue[ADC_CHANNELS_USED];
}AdcHandleConfig;

AdcHandleConfig* AdcCfgInitAndGet(void);

void AdcEnable(void);
void AdcDisable(void);
void AdcStartConversion(void);
void AdcEnableInterrupt(void);




#endif /* ADCCFG_H_ */
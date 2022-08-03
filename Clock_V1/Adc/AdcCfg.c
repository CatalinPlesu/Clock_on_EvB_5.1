#include "AdcCfg.h"

static AdcHandleConfig adcHandleConfig;

AdcHandleConfig* AdcCfgInitAndGet(void){
	
	adcHandleConfig.adcActiveChannel = 0;
	adcHandleConfig.adcChannelsCount = ADC_CHANNELS_USED;
	adcHandleConfig.adcPinValue[0] = ADC_CHANNEL_1_PIN;
	adcHandleConfig.adcPinValue[1] = ADC_CHANNEL_2_PIN;
		
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));								 // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (1<<REFS0);														 // Voltage reference from Avcc (5v)
	ADMUX |= adcHandleConfig.adcPinValue[adcHandleConfig.adcActiveChannel];		 // Defines what adc channel to read
	ADCSRA |= (1<<ADEN);														 // Turn on ADC
	ADCSRA |= (1<<ADSC);														 // Do an initial conversion because this one is the slowest and to ensure that everything is up and running

	return &adcHandleConfig;
}
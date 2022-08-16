#include "AdcCfg.h"

static AdcHandleConfig adcHandleConfig;

AdcHandleConfig* AdcCfgInitAndGet(void){
	
	adcHandleConfig.adcActiveChannel = 0;
	adcHandleConfig.adcChannelsCount = ADC_CHANNELS_USED;
	adcHandleConfig.adcpin[0] = ADC_CHANNEL_1_PIN;
	adcHandleConfig.adcpin[1] = ADC_CHANNEL_2_PIN;
	
	for (uint8_t i = 0; i < adcHandleConfig.adcChannelsCount; i++)
		DDRA &= ~(1<<adcHandleConfig.adcpin[i]);	
	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));								 // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX  |= (1<<REFS0);														 // Voltage reference from Avcc (5v)
	ADMUX  |= adcHandleConfig.adcpin[adcHandleConfig.adcActiveChannel];	 // Defines what adc channel to read
	AdcEnable();
	AdcEnableInterrupt();														 
	AdcStartConversion();														// Do an initial conversion because this one is the slowest and to ensure that everything is up and running
	return &adcHandleConfig;
}

void AdcEnable(void){
	ADCSRA |= (1<<ADEN);		// Turn on ADC
}

void AdcDisable(void){
	ADCSRA &= ~(1<<ADEN);		// Turn off ADC
}

void AdcStartConversion(void){
	ADCSRA |= (1<<ADSC);	   // Start to convert
}

void AdcEnableInterrupt(void){
	ADCSRA |= (1<<ADIE);	   // Conversion Complete	interrupt is activated.
}

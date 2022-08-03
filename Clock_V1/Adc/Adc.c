#include "Adc.h"

static AdcHandleConfig* adcHandleConfig = (void*)0;
static AdcValue adcValue = {};

//ISR(ADC_vect){
		//
	//adcValue.adcChannel[adcHandleConfig->adcActiveChannel] = ADCW;				// Saves adc value for current channel
	//
	//adcHandleConfig->adcActiveChannel++;
	//if (adcHandleConfig->adcActiveChannel == adcHandleConfig->adcChannelsCount)
		//adcHandleConfig->adcActiveChannel = 0;
	//
	//ADMUX &= 0xF0;																 // Clear the older channel that was read
	//ADMUX |= adcHandleConfig->adcPinValue[adcHandleConfig->adcActiveChannel];    // Defines the new ADC channel to be read
//}

//void AdcInit(void){
	//adcHandleConfig = AdcCfgInitAndGet();
//}

const AdcValue* GetAdcValue(){
	return &adcValue;
}

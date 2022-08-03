#include "Adc.h"

static AdcHandleConfig* adcHandleConfig;
static AdcValue adcValue = {};

//TODO vector intrerupt 
//uint16_t ADCRead(uint8_t channel){
	//ADMUX &= 0xF0;                            // Clear the older channel that was read
	//ADMUX |= channel;                            // Defines the new ADC channel to be read
	//ADCSRA |= (1<<ADSC);                            // Starts a new conversion
	//while(ADCSRA & (1<<ADSC));                        // Wait until the conversion is done
	//return ADCW;                                // Returns the ADC value of the chosen channel
//}

void AdcInit(void){
	adcHandleConfig = AdcCfgInitAndGet();
}

const AdcValue* GetAdcValue(uint8_t channel){
	return &adcValue;
}

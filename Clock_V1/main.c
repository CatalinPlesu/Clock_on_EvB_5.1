#define F_CPU 16000000
//#include "Adc.h"
#include "Rtc.h"
#include "SevSeg.h"
#include "TimerCfg.h"
#include "TimerSw.h"
#include "Control.h"
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
// ./avrdude.exe -p atmega32 -P usb -c usbasp -U flash:w:../Clock_V1/Clock_V1/Debug/Clock_V1.hex:i

static TimerSwHandle timerSwHandle;
Time currentTime = { 16, 26 };
Time oldTime = { 0, 0 };
float a = 9752;

//const AdcValue* adcValue = NULL;
ISR(ADC_vect){
	a = ADC;
}

void AdcInit();

int main(void)
{
	TimerInitCfg();
	TimerEnableCfg(true);
	
    SevSegInit();
    ControlInit();
	
	AdcInit();
	//adcValue = GetAdcValue();
	
    sei();
	
	StatusError err;
	TimerSwInitParam* pTimerSwInitParam = TimerGetIntervalPointerCfg();
	err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);
	if (err == StatusErrNone) {
		TimerSwStartup(&timerSwHandle, 1000);
	}

    while (1) {
        ControlRoutine();
        
        err = TimerSwIsExpired(&timerSwHandle);
        if (err == StatusErrTime) {
			ADCSRA |= (1<<ADSC);	   // Start to convert
			//AdcStartConversion();
			
            currentTime.minutes++;
            if(currentTime.minutes>=60){
                currentTime.minutes-=60;
                currentTime.hours++;
                if (currentTime.hours>=24)
                    currentTime.hours-=24;
            }
            TimerSwStartup(&timerSwHandle, 1000);
        }

        if (oldTime.hours != currentTime.hours || oldTime.minutes != currentTime.minutes) {
            //SevSegSetTimeVal(currentTime);
			SevSegSetFloatVal(a);
			//SevSegSetFloatVal((float)adcValue->adcChannel[0]);
            oldTime = currentTime;
        }

        SevSegRutine();
    }
}

void AdcInit(){
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));								 // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX  |= (1<<REFS0);														 // Voltage reference from Avcc (5v)
	ADMUX  |= PA7;
	ADCSRA |= (1<<ADEN);		// Turn on ADC
	ADCSRA |= (1<<ADIE);	   // Conversion Complete	interrupt is activated.
	ADCSRA |= (1<<ADSC);	   // Start to convert
}
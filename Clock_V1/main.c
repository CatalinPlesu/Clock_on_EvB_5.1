#define F_CPU 16000000
#include "Adc.h"
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
static TimerSwHandle timerSwAdcHandle;
Time currentTime = { 16, 26 };
Time oldTime = { 0, 0 };
float a = 9752;
uint8_t display = 0;

const AdcValue* adcValue = NULL;

//ISR(ADC_vect){
	//a = ADCW;
//}

int main(void)
{
	TimerInitCfg();
	TimerEnableCfg(true);
	
    SevSegInit();
    ControlInit(&display);
	
	//adcHandleConfig = AdcCfgInitAndGet();
	AdcInit();
	adcValue = GetAdcValue();
	
    sei();
	
	StatusError err;
	TimerSwInitParam* pTimerSwInitParam = TimerGetIntervalPointerCfg();
	err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);
	if (err == StatusErrNone) {
		TimerSwStartup(&timerSwHandle, 1000);
	}
	
	err = TimerSwInit(pTimerSwInitParam, &timerSwAdcHandle);
	if (err == StatusErrNone) {
		TimerSwStartup(&timerSwHandle, 8);
	}

    while (1) {
        ControlRoutine();
        
        err = TimerSwIsExpired(&timerSwHandle);
        if (err == StatusErrTime) {
			
            currentTime.minutes++;
            if(currentTime.minutes>=60){
                currentTime.minutes-=60;
                currentTime.hours++;
                if (currentTime.hours>=24)
                    currentTime.hours-=24;
            }
            TimerSwStartup(&timerSwHandle, 1000);
        }
		   err = TimerSwIsExpired(&timerSwAdcHandle);
		   if (err == StatusErrTime) {
			   AdcStartConversion();
			   SevSegSetFloatVal((float)adcValue->adcChannel[display]);
			   TimerSwStartup(&timerSwAdcHandle, 8);
		   }

        if (oldTime.hours != currentTime.hours || oldTime.minutes != currentTime.minutes) {
			AdcStartConversion();
			
            //SevSegSetTimeVal(currentTime);
			//SevSegSetFloatVal(a);
			SevSegSetFloatVal((float)adcValue->adcChannel[display]);
            oldTime = currentTime;
        }

        SevSegRutine();
    }
}

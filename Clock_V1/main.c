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
Time currentTime = { 16, 26 };
Time oldTime = { 0, 0 };
float a = 9752;

void init_Timer();

int main(void)
{
    /* Replace with your application code */
    init_Timer();
    SevSegInit();

    StatusError err;
    TimerSwInitParam* pTimerSwInitParam = TimerGetIntervalPointerCfg();
    err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);

    if (err == StatusErrNone) {
        TimerSwStartup(&timerSwHandle, 1000);
    }

    ControlInit();
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

        if (oldTime.hours != currentTime.hours || oldTime.minutes != currentTime.minutes) {
            SevSegSetTimeVal(currentTime);
			//SevSegSetFloatVal(a);
            oldTime = currentTime;
        }

        SevSegRutine();
    }
}

void init_Timer()
{

    TimerInitCfg();
    TimerEnableCfg(true);

    sei();
}

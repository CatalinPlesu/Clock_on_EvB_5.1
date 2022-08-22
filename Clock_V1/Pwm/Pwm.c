#include "Pwm.h"

#include <avr/io.h>
#include "Pwm.h"
#include "TimerSw.h"
#include "TimerCfg.h"


static uint8_t duty;
static TimerSwHandle timerSwHandle;
static PwmState pwmState = PwmStateIdle;
static uint8_t duty = 0;
static uint8_t count = 0;

void PwmInit()
{
	StatusError err;
	
	TimerSwInitParam *pTimerSwInitParam = TimerGetIntervalPointerCfg();
	
	err = TimerSwInit(pTimerSwInitParam,&timerSwHandle);
	
	if (err == StatusErrNone)
	{
		TimerSwStartup(&timerSwHandle,PWM_DATAUPDATE_MS);
	}
	
	duty = 0;
	
	TCCR2 = (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<COM20) | (1<<CS20);
	DDRD|=(0x01<<PD7);  /*set OC2 pin as output*/
	
}

void PwmToggle(){
	if(pwmState == PwmStateIdle){
		pwmState = PwmStateRunning;
		count = 0;
	}
	else
	{
		pwmState = PwmStateIdle;
	}
}

void PwmStart(){
	pwmState = PwmStateRunning;	
}

void PwmRutine()
{
	if (pwmState == PwmStateIdle)
		return;
		
	StatusError err;
	err = TimerSwIsExpired(&timerSwHandle);
	if (err == StatusErrTime && pwmState == PwmStateRunning)
	{
		OCR0=duty;
		pwmState = PwmStateRunningSilent;
		count++;
		TimerSwStartup(&timerSwHandle,PWM_DATAUPDATE_MS);
	}
	else if (err == StatusErrTime && pwmState == PwmStateRunningSilent)
	{
		OCR0=0;
		pwmState = PwmStateRunning;
		count++;
		TimerSwStartup(&timerSwHandle,3*PWM_DATAUPDATE_MS);
	}
	
	if(count > MAX_PWM_CHANGES){
		pwmState = PwmStateIdle;
	}
}

void PwmSetDuty(uint8_t dutycycle){
	duty = dutycycle;
}
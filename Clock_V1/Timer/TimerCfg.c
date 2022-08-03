/*
 * TimerCfg.c
 *
 * Created: 7/6/2022 6:11:44 PM
 *  Author: alexei.guriev
 */ 

/*******************************************************************************
 * includes
 ******************************************************************************/
#include "TimerCfg.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>


/*******************************************************************************
 * local vars
 ******************************************************************************/

/*******************************************************************************
 * local functions
 ******************************************************************************/

/*******************************************************************************
 * local functions
 ******************************************************************************/

/*******************************************************************************
 * functions
 ******************************************************************************/

TimerSwInitParam timerSwInitParam;

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	timerSwInitParam.interval++;
	TCNT1 = TTIMER_CFG_COUNTER_INIT_STATE;   // for 1 sec at 16 MHz  63974
}

void TimerInitCfg(){
	TCNT1 = TTIMER_CFG_COUNTER_INIT_STATE;   // for 1 sec at 16 MHz
	TCCR1A = 0x00;
	TCCR1B = (1<<CS10);  // Timer mode with 1 prescler
	
	timerSwInitParam.interval = 0;
}

void TimerEnableCfg(bool state){
	
	if(state == false)
	{
		TIMSK &= ~(1UL << TOIE1); // Disable timer1 overflow interrupt(TOIE1)			
	}
	else
	{
		TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	}
}

TimerSwInitParam* TimerGetIntervalPointerCfg(void)
{
	return &timerSwInitParam;
}
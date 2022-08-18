#include <avr/io.h>
#include <avr/interrupt.h>
#include "Led.h"

ISR(INT1_vect)
{
	LedAllOn();
	//RtcCfgAlamFlagClear();
}

void IntInit(void){
	GICR = (0x01 << INT1);             /* Enable INT1*/
	MCUCR = (1 << ISC11 | 0 << ISC10); /* Trigger INT1 on rising edge */
}

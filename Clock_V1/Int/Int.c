#include <avr/io.h>
#include <avr/interrupt.h>
#include "Led.h"
#include "Rtc.h"
#include "SevSeg.h"

//ISR(INT0_vect)
//{		
	//if(GIFR&(0x01<<INTF0)){
		//LedAllOff();
		//LedOn(4);
	//}
	//// power on mcu
//}

ISR(INT1_vect)
{
	LedAllOff();
	LedOn(4);
	//RtcCfgAlamFlagClear();
}

void IntInit(void){
	//EnableInt(0);
	EnableInt(1);
	//MCUCR |= (1 << ISC01 | 1 << ISC00);  //  Trigger INT0 on rising edge
	//MCUCR |= (1 << ISC11 | 1 << ISC10); /*  Trigger INT1 on rising edge */
}

void EnableInt(uint8_t n){
	switch (n){
		case 0:
		GICR |= (0x01 << INT0); break;
		case 1:
		GICR |= (0x01 << INT1); break;
		case 2:
		GICR |= (0x01 << INT2); break;
	}
}

void DisableInt(uint8_t n){
	switch (n){
		case 0:
		GICR &= ~(0x01 << INT0); break;
		case 1:
		GICR &= ~(0x01 << INT1); break;
		case 2:
		GICR &= ~(0x01 << INT2); break;
	}
}
#include <avr/io.h>

#ifndef SEVSEGCFG_H_
#define SEVSEGCFG_H_

#define COMUN_POWER_SUPPLY	0xAA
#define COMUN_GROUND		0x55

#define SEVSEG_SEGMENT_MODE	COMUN_POWER_SUPPLY
//#define SEVSEG_SEGMENT_MODE	COMUN_GROUND

#define SEVSEG_TIMER_MS	4


#define SEVSEG_0_DDR DDRB
#define SEVSEG_1_DDR DDRB
#define SEVSEG_2_DDR DDRB
#define SEVSEG_3_DDR DDRB
#define SEVSEG_4_DDR DDRB
#define SEVSEG_5_DDR DDRB
#define SEVSEG_6_DDR DDRB
#define SEVSEG_7_DDR DDRB

#define SEVSEG_0_PORT PORTB
#define SEVSEG_1_PORT PORTB
#define SEVSEG_2_PORT PORTB
#define SEVSEG_3_PORT PORTB
#define SEVSEG_4_PORT PORTB
#define SEVSEG_5_PORT PORTB
#define SEVSEG_6_PORT PORTB
#define SEVSEG_7_PORT PORTB

#define SEVSEG_0_PORT_VALUE PB0
#define SEVSEG_1_PORT_VALUE PB1
#define SEVSEG_2_PORT_VALUE PB2
#define SEVSEG_3_PORT_VALUE PB3
#define SEVSEG_4_PORT_VALUE PB4
#define SEVSEG_5_PORT_VALUE PB5
#define SEVSEG_6_PORT_VALUE PB6
#define SEVSEG_7_PORT_VALUE PB7

#define SEVSEG_DIGIT_1_DDR DDRC
#define SEVSEG_DIGIT_2_DDR DDRC
#define SEVSEG_DIGIT_3_DDR DDRC
#define SEVSEG_DIGIT_4_DDR DDRC

#define SEVSEG_DIGIT_1_PORT PORTC
#define SEVSEG_DIGIT_2_PORT PORTC
#define SEVSEG_DIGIT_3_PORT PORTC
#define SEVSEG_DIGIT_4_PORT PORTC

#define SEVSEG_DIGIT_1_PORT_VALUE PC2
#define SEVSEG_DIGIT_2_PORT_VALUE PC3
#define SEVSEG_DIGIT_3_PORT_VALUE PC4
#define SEVSEG_DIGIT_4_PORT_VALUE PC5

#define SEVSEG_DIGITS_COUNT 4

typedef struct
{
	volatile uint8_t* ddrSeg[8];
	volatile uint8_t* portSeg[8];
	uint8_t portSegValue[8];

	volatile uint8_t* ddrDIgit[SEVSEG_DIGITS_COUNT];
	volatile uint8_t* portDigit[SEVSEG_DIGITS_COUNT];
	uint8_t portDigitValue[SEVSEG_DIGITS_COUNT];

	uint8_t digitsCount;
}SevSegHandleConfig;

SevSegHandleConfig* SevSegCfgInitAndGet(void);

void SevSegCfgAllDigitsOff(void);

void SevSegCfgSetDigitValue(uint8_t value);

void SevSegCfgDigitOn(uint8_t digit);


#endif /* DISP7SEGCFG_H_ */

#include <avr/io.h>

#ifndef DISP7SEGCFG_H_
#define DISP7SEGCFG_H_

#define COMUN_POWER_SUPPLY	0xAA
#define COMUN_GROUND		0x55

#define DISPLAY_7_SEGMENT_MODE	COMUN_POWER_SUPPLY
//#define DISPLAY_7_SEGMENT_MODE	COMUN_GROUND

#define DISPLAY_7_SEGMENT_TIMER_MS	4

#define DISPLAY_7_SEGMENT_PORT_DIR DDRB
#define DISPLAY_7_SEGMENT_DIGIT_PORT_DIR DDRC

#define DISPLAY_7_SEGMENT_PORT PORTB
#define DISPLAY_7_SEGMENT_DIGIT_PORT PORTC

#define DISPLAY_7_SEGMENT_DIGIT_1_PIN PC2
#define DISPLAY_7_SEGMENT_DIGIT_2_PIN PC3
#define DISPLAY_7_SEGMENT_DIGIT_3_PIN PC4
#define DISPLAY_7_SEGMENT_DIGIT_4_PIN PC5

#define DISPLAY_7_SEGMENT_DIGITS_COUNT 4

typedef struct
{
	uint8_t port;
	uint8_t portDig;
	uint8_t digitsCount;
	uint8_t digitsPinValue[DISPLAY_7_SEGMENT_DIGITS_COUNT];
}SevSegHandleConfig;

SevSegHandleConfig* SevSegCfgInitAndGet(void);

void SevSegCfgAllDigitsOff(void);

void SevSegCfgSetDigitValue(uint8_t value);

void SevSegCfgDigitOn(uint8_t digit);


#endif /* DISP7SEGCFG_H_ */

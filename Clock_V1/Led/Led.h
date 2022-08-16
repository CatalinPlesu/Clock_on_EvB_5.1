#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include "LedCfg.h"

typedef struct
{
	uint8_t count;
	uint8_t pinValue[LED_COUNT];
}LedHandleConfig;

void LedInit(void);

// All Leds
void LedAllOn(void);
void LedAllOff(void);
void LedAllToggle(void);

// Single Led
void LedOn(uint8_t index);
void LedOff(uint8_t index);
void LedToggle(uint8_t index);

#endif /* LED_H_ */
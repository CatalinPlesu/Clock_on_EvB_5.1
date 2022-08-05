#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define PORT_DIRECTION_LED DDRA
#define PORT_LED PORTA
#define PIN_LED_CLOCK PA0
#define PIN_LED_TIMER PA1
#define PIN_LED_ALARM PA2
#define PIN_LED_CONTDOWN PA3
#define PIN_LED_TEMPERATURE PA4
#define LED_COUNT 5

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
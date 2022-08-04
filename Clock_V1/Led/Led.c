#include "Led.h"

static LedHandleConfig ledHandleConfig;

void LedInit()
{
	ledHandleConfig.pinValue[0] = PIN_LED_CLOCK;
	ledHandleConfig.pinValue[1] = PIN_LED_TIMER;
	ledHandleConfig.pinValue[2] = PIN_LED_ALARM;
	ledHandleConfig.pinValue[3] = PIN_LED_CONTDOWN;
	ledHandleConfig.pinValue[4] = PIN_LED_TEMPERATURE;
	ledHandleConfig.count = LED_COUNT;

	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		PORT_DIRECTION_LED |= (1 << ledHandleConfig.pinValue[i]);
	
	LedAllOff();
}

void LedAllOn(void)
{
	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		PORT_LED &= ~(1 << ledHandleConfig.pinValue[i]);
}

void LedAllOff(void)
{
	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		PORT_LED |= (1 << ledHandleConfig.pinValue[i]);
}

void LedAllToggle(void)
{
	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		PORT_LED ^= (1 << ledHandleConfig.pinValue[i]);
}


void LedOn(uint8_t index)
{
	PORT_LED &= ~(1 << ledHandleConfig.pinValue[index]);
}

void LedOff(uint8_t index)
{
	PORT_LED |= (1 << ledHandleConfig.pinValue[index]);
}

void LedToggle(uint8_t index)
{
	PORT_LED ^= (1 << ledHandleConfig.pinValue[index]);
}
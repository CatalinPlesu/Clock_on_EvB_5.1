#include "Led.h"

static LedHandleConfig ledHandleConfig;

void LedInit()
{
	LedHandleConfig led_handle_config = {
		.ddr = {&LED0_DDR, &LED1_DDR, &LED2_DDR, &LED3_DDR, &LED4_DDR},
		.port = {&LED0_PORT, &LED1_PORT, &LED2_PORT, &LED3_PORT, &LED4_PORT},
		.pinValue = {LED0_PIN_VALUE, LED1_PIN_VALUE, LED2_PIN_VALUE,
			LED3_PIN_VALUE, LED4_PIN_VALUE},
		.count = LED_COUNT,
	};
	ledHandleConfig = led_handle_config;

	// makes led ports output
	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		*ledHandleConfig.ddr[i] |= (1 << ledHandleConfig.pinValue[i]);
	
	LedAllOff();
}

void LedAllOn(void)
{
	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		*ledHandleConfig.port[i] &= ~(1 << ledHandleConfig.pinValue[i]);
}

void LedAllOff(void)
{
	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		*ledHandleConfig.port[i] |= (1 << ledHandleConfig.pinValue[i]);
}

void LedAllToggle(void)
{
	for (uint8_t i = 0; i < ledHandleConfig.count; i++)
		*ledHandleConfig.port[i] ^= (1 << ledHandleConfig.pinValue[i]);
}


void LedOn(uint8_t index)
{
	*ledHandleConfig.port[index] &= ~(1 << ledHandleConfig.pinValue[index]);
}

void LedOff(uint8_t index)
{
	*ledHandleConfig.port[index] |= (1 << ledHandleConfig.pinValue[index]);
}

void LedToggle(uint8_t index)
{
	*ledHandleConfig.port[index] ^= (1 << ledHandleConfig.pinValue[index]);
}

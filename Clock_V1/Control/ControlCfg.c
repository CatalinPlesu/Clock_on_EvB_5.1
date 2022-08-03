#import "ControlCfg.h"

static ControlHandleConfig controlHandleConfig;

ControlHandleConfig* ControlCfgInitAndGet()
{
    controlHandleConfig.portLed = &PORT_LED;
    controlHandleConfig.portButton = &PORT_BUTTON;
    controlHandleConfig.pinButton = &PIN_BUTTON;

    controlHandleConfig.ledPinValue[0] = PIN_LED_CLOCK;
    controlHandleConfig.ledPinValue[1] = PIN_LED_TIMER;
    controlHandleConfig.ledPinValue[2] = PIN_LED_ALARM;
    controlHandleConfig.ledPinValue[3] = PIN_LED_CONTDOWN;
    controlHandleConfig.ledPinValue[4] = PIN_LED_TEMPERATURE;
    controlHandleConfig.ledCount = LED_COUNT;

    controlHandleConfig.buttonPinValue[0] = PIN_BUTTON_POWER;
    controlHandleConfig.buttonPinValue[1] = PIN_BUTTON_NEXT;
    controlHandleConfig.buttonPinValue[2] = PIN_BUTTON_TOGGLE;
    controlHandleConfig.buttonPinValue[3] = PIN_BUTTON_INCREASE;
    controlHandleConfig.buttonPinValue[4] = PIN_BUTTON_DECREASE;
    controlHandleConfig.buttonPinValue[5] = PIN_BUTTON_OK;
    controlHandleConfig.buttonPinValue[6] = PIN_BUTTON_ABORT;
    controlHandleConfig.buttonCount = BUTTON_COUNT;

    for (uint8_t i = 0; i < controlHandleConfig.ledCount; i++)
        PORT_DIRECTION_LED |= (1 << controlHandleConfig.ledPinValue[i]);

    for (uint8_t i = 0; i < controlHandleConfig.ledCount; i++)
        PORT_DIRECTION_BUTTON &= ~(1 << controlHandleConfig.ledPinValue[i]);
		
	for(uint8_t i = 0; i < controlHandleConfig.buttonCount; i++)
		*controlHandleConfig.portButton |= (1 << controlHandleConfig.buttonPinValue[i]);

    return &controlHandleConfig;
}

void ControlCfgAllLedsOff(void)
{
    for (uint8_t i = 0; i < controlHandleConfig.ledCount; i++)
        *controlHandleConfig.portLed |= (1 << controlHandleConfig.ledPinValue[i]);
}

void ControlCfgAllLedOff(uint8_t index)
{
    *controlHandleConfig.portLed  |= (1 << controlHandleConfig.ledPinValue[index]);
}

void ControlCfgAllLedsOn(void)
{
    for (uint8_t i = 0; i < controlHandleConfig.ledCount; i++)
        *controlHandleConfig.portLed &= ~(1 << controlHandleConfig.ledPinValue[i]);
}

void ControlCfgAllLedOn(uint8_t index)
{
    *controlHandleConfig.portLed &= ~(1 << controlHandleConfig.ledPinValue[index]);
}


#ifndef CONTROLCFG_H_
#define CONTROLCFG_H_

#include <avr/io.h>

#define PORT_DIRECTION_LED DDRA
#define PORT_LED PORTA
#define PIN_LED_CLOCK PA0
#define PIN_LED_TIMER PA1
#define PIN_LED_ALARM PA2
#define PIN_LED_CONTDOWN PA3
#define PIN_LED_TEMPERATURE PA4
#define LED_COUNT 5

#define PORT_DIRECTION_BUTTON DDRD
#define PORT_BUTTON PORTD
#define PIN_BUTTON PIND
#define PIN_BUTTON_POWER PD0
#define PIN_BUTTON_NEXT PD1
#define PIN_BUTTON_TOGGLE PD2
#define PIN_BUTTON_INCREASE PD3
#define PIN_BUTTON_DECREASE PD4
#define PIN_BUTTON_OK PD5
#define PIN_BUTTON_ABORT PD6
#define BUTTON_COUNT 7

typedef enum
{
    ButtonPower,
    ButtonNext, 
    ButtonToggle,
    ButtonIncrease,
    ButtonDecrease,
    ButtonOk,
    ButtonAbort
}Buttons;

typedef enum
{
	LedClock,
    LedTimer,
    LedAlarm,
    LedContdown,
    LedTemperature
}LedMeaning;

typedef enum
{
    DisplayView,
    DisplaySettings,
}DisplayMode;

typedef struct
{
	uint8_t *portLed;
	uint8_t *portButton;
	uint8_t *pinButton;
	uint8_t ledCount;
	uint8_t buttonCount;
	uint8_t ledPinValue[LED_COUNT];
	uint8_t buttonPinValue[BUTTON_COUNT];
}ControlHandleConfig;


ControlHandleConfig* ControlCfgInitAndGet(void);

void ControlCfgAllLedsOn(void);
void ControlCfgAllLedsOff(void);
void ControlCfgAllLedOn(uint8_t index);
void ControlCfgAllLedOff(uint8_t index);

#endif

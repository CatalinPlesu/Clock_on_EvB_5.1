#ifndef MAIN_H_
#define MAIN_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

typedef enum
{	DeviceStateStartup,
	DeviceStateInit,
	DeviceStateSleep,
	DeviceStateWake,
}DeviceState;

typedef enum
{
	DeviceDisplayStateClock,
	DeviceDisplayStateTimer,
	DeviceDisplayStateAlarm,
	DeviceDisplayStateCountdown,
	DeviceDisplayStateTemperature,
}DeviceDisplayState;

typedef enum
{
	DisplayStateOff,
	DisplayStateNormal,
	DisplayStateEdit,
}DisplayState;

typedef enum
{
	EditStateHoursTens,
	EditStateHoursUnits,
	EditStateMinutesTens,
	EditStateMinutesUnits,
	EditStateSecondsTens,
	EditStateSecondsUnits,
}EditState;


void ButtonPowerFunction(uint8_t index);
void ButtonNextFunction(uint8_t index);
void ButtonNextDigitFunction(uint8_t index);
void ButtonIncreaseFunction(uint8_t index);
void ButtonDecreaseFunction(uint8_t index);
void ButtonOkFunction(uint8_t index);
void ButtonEditFunction(uint8_t index);

void SevSegRefresh(bool optional);


void DeviceDisplayStateLedNormal(void);
void DeviceDisplayStateLedEdit(void);
void BuzzerFunction(void);

#endif /* MAIN_H_ */
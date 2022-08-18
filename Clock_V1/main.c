// ./avrdude.exe -p atmega32 -P usb -c usbasp -U flash:w:../Clock_V1/Clock_V1/Debug/Clock_V1.hex:i

#include "main.h"
#include "Adc.h"
#include "Button.h"
#include "Ds18b20.h"
#include "Led.h"
#include "Rtc.h"
#include "SevSeg.h"
#include "TimerCfg.h"
#include "TimerSw.h"

DeviceState deviceState = DeviceStateStartup;
DeviceDisplayState deviceDisplayState = DeviceDisplayStateClock;
// DisplayState displayState = DisplayStateOff;
DisplayState displayState = DisplayStateNormal;
EditState editState = EditStateHoursTens;

static TimerSwHandle timerSwHandle;
const AdcValue* adcValue = NULL;

Time* ptrTimeTrackers[4] = { NULL, NULL, NULL, NULL };
Time oldTime = {};

Time desiredTimeTrackers[] = {
	[DeviceDisplayStateClock] = { (1 << 4) | 2, 0 },
	[DeviceDisplayStateTimer] = { 0, 0 },
	[DeviceDisplayStateAlarm] = { (1 << 4) | 2, 0 },
	[DeviceDisplayStateCountdown] = { 0, 5 }
};

int8_t temperature = 5;
int8_t oldTemperature = 0;

void ButtonPowerFunction(uint8_t index);
void ButtonNextFunction(uint8_t index);
void ButtonNextDigitFunction(uint8_t index);
void ButtonIncreaseFunction(uint8_t index);
void ButtonDecreaseFunction(uint8_t index);
void ButtonOkFunction(uint8_t index);
void ButtonEditFunction(uint8_t index);

void SevSegRefresh(bool optional);

static ButtonFunctionPtr buttonFunctionPtr = {
	ButtonPowerFunction,
	ButtonNextFunction,
	ButtonNextDigitFunction,
	ButtonIncreaseFunction,
	ButtonDecreaseFunction,
	ButtonOkFunction,
	ButtonEditFunction
};

void DeviceDisplayStateLedNormal(void);
void DeviceDisplayStateLedEdit(void);
void BuzzerFunction(void);

int main(void)
{
	StatusError err;
	TimerSwInitParam* pTimerSwInitParam = TimerGetIntervalPointerCfg();

	if (deviceState == DeviceStateStartup) {
		deviceState = DeviceStateInit;

	TimerInitCfg();
	TimerEnableCfg(true);

		SevSegInit();
		ButtonInit(&buttonFunctionPtr);
		LedInit();

		AdcInit();
		adcValue = GetAdcValue();

		RtcInit(BuzzerFunction);
		ptrTimeTrackers[DeviceDisplayStateClock] = GetRtcTime();
		ptrTimeTrackers[DeviceDisplayStateTimer] = GetRtcTimer();
		ptrTimeTrackers[DeviceDisplayStateAlarm] = GetRtcAlarm();
		ptrTimeTrackers[DeviceDisplayStateCountdown] = GetRtcCountdown();
		// RtcSetTime(RtcCreateTime(13, 31));

		sei();

		err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);
		if (err == StatusErrNone) {
			TimerSwStartup(&timerSwHandle, 1000);
		}
		DeviceDisplayStateLedNormal();
	}

	while (1) {
		ButtonRoutine();
		RtcReadTime();

		// err = TimerSwIsExpired(&timerSwHandle);
		// if (err == StatusErrTime) {
		// TimerSwStartup(&timerSwHandle, 1000);
		// }
		if (deviceDisplayState == DeviceDisplayStateTimer) {
			RtcTimerRoutine();
		}

		SevSegRefresh(true);

		if (displayState == DisplayStateOff)
		SevSegCfgAllDigitsOff();
		else
		SevSegRutine();
	}
}

void ButtonPowerFunction(uint8_t index)
{
	if (displayState == DisplayStateOff) {
		displayState = DisplayStateNormal;
		SevSegRefresh(false);
		DeviceDisplayStateLedNormal();
		} else {
		displayState = DisplayStateOff;
		LedAllOff();
	}
}

void ButtonNextFunction(uint8_t index)
{
	if (displayState != DisplayStateNormal)
	return;

	if (deviceDisplayState == DeviceDisplayStateTemperature)
	deviceDisplayState = 0;
	else
	deviceDisplayState++;
	DeviceDisplayStateLedNormal();
	SevSegRefresh(false);
}

void ButtonNextDigitFunction(uint8_t index)
{
	if (displayState != DisplayStateEdit)
	return;

	if (editState == EditStateSecondsUnits) {
		editState = EditStateHoursTens;
		} else {
		editState++;
	}
}

void ButtonIncreaseFunction(uint8_t index)
{
	if (displayState != DisplayStateEdit)
	return;

	// new time incrementation

	if (deviceDisplayState == DeviceDisplayStateTemperature) {
		temperature++;
		} else {
		uint8_t seconds = seconds_to_uint8_t(desiredTimeTrackers[deviceDisplayState].seconds);
		uint8_t minutes = minutes_to_uint8_t(desiredTimeTrackers[deviceDisplayState].minutes);
		uint8_t hours = hours_to_uint8_t(desiredTimeTrackers[deviceDisplayState].hours);
		
		switch (editState) {
			case EditStateHoursTens:
			hours+=10;break;
			case EditStateHoursUnits:
			hours+=1;break;
			case EditStateMinutesTens:
			minutes+=10;break;
			case EditStateMinutesUnits:
			minutes+=1;break;
			case EditStateSecondsTens:
			seconds+=10;break;
			case EditStateSecondsUnits:
			seconds+=1;break;
			default:break;
		}
		
		desiredTimeTrackers[deviceDisplayState] = RtcCreateTime(hours, minutes, seconds);
	}
	SevSegRefresh(false);
}

void ButtonDecreaseFunction(uint8_t index)
{
	if (displayState != DisplayStateEdit)
	return;

	if (deviceDisplayState == DeviceDisplayStateTemperature) {
		temperature--;
		} else {
		uint8_t seconds = seconds_to_uint8_t(desiredTimeTrackers[deviceDisplayState].seconds);
		uint8_t minutes = minutes_to_uint8_t(desiredTimeTrackers[deviceDisplayState].minutes);
		uint8_t hours = hours_to_uint8_t(desiredTimeTrackers[deviceDisplayState].hours);
		
		switch (editState) {
			case EditStateHoursTens:
			hours-=10;break;
			case EditStateHoursUnits:
			hours-=1;break;
			case EditStateMinutesTens:
			minutes-=10;break;
			case EditStateMinutesUnits:
			minutes-=1;break;
			case EditStateSecondsTens:
			seconds-=10;break;
			case EditStateSecondsUnits:
			seconds-=1;break;
			default:break;
		}
		
		desiredTimeTrackers[deviceDisplayState] = RtcCreateTime(hours, minutes, seconds);
	}
	SevSegRefresh(false);
}

void ButtonOkFunction(uint8_t index)
{
	if (displayState == DisplayStateEdit) {
		displayState = DisplayStateNormal;
		if (deviceDisplayState == DeviceDisplayStateClock) {
			RtcSetTime(desiredTimeTrackers[deviceDisplayState]);
		}
		else if (deviceDisplayState == DeviceDisplayStateAlarm) {
			RtcCfgWriteAlarm(desiredTimeTrackers[deviceDisplayState]);
		}
		DeviceDisplayStateLedNormal();
		SevSegRefresh(false);
		} else if (deviceDisplayState == DeviceDisplayStateTimer) {
		RtcTimerToggle();
		SevSegRefresh(false);
	}
	else if (deviceDisplayState == DeviceDisplayStateAlarm) {
		RtcAlarmToggle();
	}
}

void ButtonEditFunction(uint8_t index)
{
	if (deviceDisplayState == DeviceDisplayStateTimer &&
	displayState == DisplayStateNormal) {
		RtcTimerRestart();
		SevSegRefresh(false);
	}
	else if (displayState == DisplayStateNormal) {
		displayState = DisplayStateEdit;
		DeviceDisplayStateLedEdit();
		if (deviceDisplayState == DeviceDisplayStateTemperature) {
			oldTemperature = temperature;
			}
		 else {
			desiredTimeTrackers[deviceDisplayState] = (*ptrTimeTrackers)[deviceDisplayState];
		}
		SevSegRefresh(false);
		} else if (displayState == DisplayStateEdit) {
		displayState = DisplayStateNormal;
		SevSegRefresh(false);
		DeviceDisplayStateLedNormal();
	}
}

void DeviceDisplayStateLedNormal(void)
{
	LedAllOff();
	LedOn(deviceDisplayState);
}

void DeviceDisplayStateLedEdit(void)
{
	LedAllOn();
	LedOff(deviceDisplayState);
}

void SevSegRefresh(bool optional)
{
	static RtcDisplayData rtcDisplayData = {};
	if (deviceDisplayState == DeviceDisplayStateTemperature) {
		if (oldTemperature != temperature || !optional) {
			oldTemperature = temperature;
			SevSegSetTemperatureVal(temperature);
		}
		} else {
		if (displayState == DisplayStateNormal) {
			if (oldTime.hours != (*ptrTimeTrackers)[deviceDisplayState].hours
			|| oldTime.minutes != (*ptrTimeTrackers)[deviceDisplayState].minutes
			|| oldTime.seconds != (*ptrTimeTrackers)[deviceDisplayState].seconds
			|| !optional) {
				oldTime = (*ptrTimeTrackers)[deviceDisplayState];
				if (deviceDisplayState == DeviceDisplayStateTimer) {
						rtcDisplayData = RtcExtractTime(*ptrTimeTrackers[deviceDisplayState], true, 0, false);
					} 
				else {
					rtcDisplayData = RtcExtractTime(*ptrTimeTrackers[deviceDisplayState], false, 0, false);
				}
				SevSegSetTimeVal(rtcDisplayData.digit, rtcDisplayData.dots);
			}
			} else if (displayState == DisplayStateEdit) {
				rtcDisplayData = RtcExtractTime(desiredTimeTrackers[deviceDisplayState], false, editState, true);
				SevSegSetTimeVal(rtcDisplayData.digit, rtcDisplayData.dots);
		}
	}
}

void BuzzerFunction(void)
{
	// call buzzer
	LedAllOn();
}

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
#include "Int.h"

DeviceState deviceState = DeviceStateStartup;
DeviceDisplayState deviceDisplayState = DeviceDisplayStateClock;
// DisplayState displayState = DisplayStateOff;
DisplayState displayState = DisplayStateNormal;
EditState editState = EditStateHoursTens;

static TimerSwHandle timerSwHandle;
const AdcValue* adcValue = NULL;

Time* ptrTimeTrackers[4] = { NULL, NULL, NULL, NULL };
Time oldTime = {};

Time desiredTimeTrackers[] = {};

int8_t temperature = 5;
int8_t oldTemperature = 0;

#define RTC_MAX_COUNTER_S 60
uint8_t rtcCounter = 0;


static ButtonFunctionPtr buttonFunctionPtr = {
	ButtonPowerFunction,
	ButtonNextFunction,
	ButtonNextDigitFunction,
	ButtonIncreaseFunction,
	ButtonDecreaseFunction,
	ButtonOkFunction,
	ButtonEditFunction
};


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

		RtcInit();
		ptrTimeTrackers[DeviceDisplayStateClock] = GetRtcTime();
		ptrTimeTrackers[DeviceDisplayStateTimer] = GetRtcTimer();
		ptrTimeTrackers[DeviceDisplayStateAlarm] = GetRtcAlarm();
		ptrTimeTrackers[DeviceDisplayStateCountdown] = GetRtcCountdown();
		
		//IntInit();

		sei();

		err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);
		if (err == StatusErrNone) {
			TimerSwStartup(&timerSwHandle, 1000);
		}
		DeviceDisplayStateLedNormal();
		RtcReadTime();
	}

	while (1) {
		ButtonRoutine();

		 err = TimerSwIsExpired(&timerSwHandle);
		 if (err == StatusErrTime) {
			RtcTimeTick();
			rtcCounter++;
			if (deviceDisplayState == DeviceDisplayStateTimer) {
				RtcTimerRoutine();
			}
			if (deviceDisplayState == DeviceDisplayStateCountdown) {
				RtcCountdownRoutine();
			}
		    TimerSwStartup(&timerSwHandle, 1000);
		 }
		 
		if(rtcCounter > RTC_MAX_COUNTER_S){
			RtcReadTime();
			rtcCounter = 0;
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
	
	if (deviceDisplayState == DeviceDisplayStateTemperature){
		deviceDisplayState = 0;
	}
	else{
		deviceDisplayState++;
	}
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
			RtcAlarmSet(desiredTimeTrackers[deviceDisplayState]);
		}
		else if (deviceDisplayState == DeviceDisplayStateCountdown) {
			RtcCountdownSet(desiredTimeTrackers[deviceDisplayState]);
		}
		DeviceDisplayStateLedNormal();
		SevSegRefresh(false);
		}
	else if (deviceDisplayState == DeviceDisplayStateTimer) {
		RtcTimerToggle();
		SevSegRefresh(false);
	}
	else if (deviceDisplayState == DeviceDisplayStateAlarm) {
		RtcAlarmToggle();
		DeviceDisplayStateLedNormal();
	}
	else if (deviceDisplayState == DeviceDisplayStateCountdown) {
		RtcCountdownToggle();
	}
}

void ButtonEditFunction(uint8_t index)
{
	if (deviceDisplayState == DeviceDisplayStateTimer) {
		RtcTimerRestart();
		SevSegRefresh(false);
	}
	else if (displayState == DisplayStateNormal) {
		editState = EditStateHoursTens;
		displayState = DisplayStateEdit;
		DeviceDisplayStateLedEdit();
		if (deviceDisplayState != DeviceDisplayStateTemperature) {
			desiredTimeTrackers[deviceDisplayState] = *ptrTimeTrackers[deviceDisplayState];
		}
		SevSegRefresh(false);
	}
	 else if (displayState == DisplayStateEdit) {
		displayState = DisplayStateNormal;
		SevSegRefresh(false);
		DeviceDisplayStateLedNormal();
	}
}

void DeviceDisplayStateLedNormal(void)
{
	LedAllOff();
	LedOn(deviceDisplayState);
	if(deviceDisplayState==DeviceDisplayStateAlarm){
		RtcAlarmIndicator();
	}
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
		} 
	else {
		if (displayState == DisplayStateNormal) {
			if (oldTime.hours != ptrTimeTrackers[deviceDisplayState]->hours
			|| oldTime.minutes != ptrTimeTrackers[deviceDisplayState]->minutes
			|| oldTime.seconds != ptrTimeTrackers[deviceDisplayState]->seconds
			|| !optional) {
				
				oldTime = *ptrTimeTrackers[deviceDisplayState];
				
				if (deviceDisplayState == DeviceDisplayStateTimer || deviceDisplayState == DeviceDisplayStateCountdown) {
					rtcDisplayData = RtcExtractTime(*ptrTimeTrackers[deviceDisplayState], true, 0);
					} 
				else {
					rtcDisplayData = RtcExtractTime(*ptrTimeTrackers[deviceDisplayState], false, 0);
				}
				SevSegSetTimeVal(rtcDisplayData.digit, rtcDisplayData.dots & 0x0f); // 0x0f to disable blink bits
				}
			} 
			else if (displayState == DisplayStateEdit) {
				rtcDisplayData = RtcExtractTime(desiredTimeTrackers[deviceDisplayState], false, editState);
				SevSegSetTimeVal(rtcDisplayData.digit, rtcDisplayData.dots);
		}
	}
}

void BuzzerFunction(void)
{
	// call buzzer
	LedAllOn();
}

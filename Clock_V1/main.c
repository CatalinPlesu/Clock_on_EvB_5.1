// ./avrdude.exe -p atmega32 -P usb -c usbasp -U flash:w:../Clock_V1/Clock_V1/Debug/Clock_V1.hex:i

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

#include "Adc.h"
#include "Button.h"
#include "Led.h"
#include "Rtc.h"
#include "SevSeg.h"
#include "TimerCfg.h"
#include "TimerSw.h"
#include "config.h"

DeviceState deviceState = DeviceStateStartup;
//DeviceDisplayState deviceDisplayState = DeviceDisplayStateTemperature;
DeviceDisplayState deviceDisplayState = DeviceDisplayStateClock; 
/* DisplayState displayState = DisplayStateOff; */
DisplayState displayState = DisplayStateNormal;
EditState editState = EditStateHoursTens;

static TimerSwHandle timerSwHandle;
const AdcValue* adcValue = NULL;

Time* ptrTimeTrackers[4] = {NULL, NULL, NULL, NULL};

Time desiredTimeTrackers[] = {
	[DeviceDisplayStateClock] = { 1, 2 },
	[DeviceDisplayStateTimer] = { 3, 4 },
	[DeviceDisplayStateAlarm] = { 5, 6 },
	[DeviceDisplayStateCountdown] = { 7, 8 }
};

uint16_t temperature = 25;

void ButtonPowerFunction(uint8_t index);
void ButtonNextFunction(uint8_t index);
void ButtonNextDigitFunction(uint8_t index);
void ButtonIncreaseFunction(uint8_t index);
void ButtonDecreaseFunction(uint8_t index);
void ButtonOkFunction(uint8_t index);
void ButtonEditFunction(uint8_t index);

void SevSegRefresh(void);

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
        //RtcSetTime(RtcCreateTime(13, 31));

        sei();

        err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);
        if (err == StatusErrNone) {
            TimerSwStartup(&timerSwHandle, 1000);
        }
    }

    while (1) {
        ButtonRoutine();

        err = TimerSwIsExpired(&timerSwHandle);
        if (err == StatusErrTime && !(deviceDisplayState == DeviceDisplayStateClock && displayState == DisplayStateEdit)) {

            SevSegRefresh();

            TimerSwStartup(&timerSwHandle, 1000);
            RtcReadTime();
        }

        /* SevSegRefresh(); */

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
    SevSegRefresh();
}

void ButtonNextDigitFunction(uint8_t index)
{
    if (displayState != DisplayStateEdit)
        return;

    if (editState == EditStateMinutesUnits)
        editState = EditStateHoursTens;
    else
        editState++;
}

void ButtonIncreaseFunction(uint8_t index)
{
	static void (*ptrIncreaseFunction[])(Time*) = {
		[EditStateHoursTens] =     RtcHoursTensIncrease,
		[EditStateHoursUnits] =    RtcHoursUnitsIncrease,
		[EditStateMinutesTens] =   RtcMinutesTensIncrease,
		[EditStateMinutesUnits] =  RtcMinutesUnitsIncrease,
	};

    if (displayState != DisplayStateEdit)
        return;

    if (deviceDisplayState == DeviceDisplayStateTemperature) {
        temperature++;
    } else {
		ptrIncreaseFunction[editState](&desiredTimeTrackers[deviceDisplayState]);
    }
    SevSegRefresh();
}

void ButtonDecreaseFunction(uint8_t index)
{
	static void (*ptrDecreaseFunction[])(Time*) = {
		[EditStateHoursTens] =     RtcHoursTensDecrease,
		[EditStateHoursUnits] =    RtcHoursUnitsDecrease,
		[EditStateMinutesTens] =   RtcMinutesTensDecrease,
		[EditStateMinutesUnits] =  RtcMinutesUnitsDecrease,
	};

    if (displayState != DisplayStateEdit)
        return;

    if (deviceDisplayState == DeviceDisplayStateTemperature) {
        temperature--;
    } else {
		ptrDecreaseFunction[editState](&desiredTimeTrackers[deviceDisplayState]);
    }
    SevSegRefresh();
}

void ButtonOkFunction(uint8_t index)
{
    displayState = DisplayStateNormal;
	if(deviceDisplayState==DeviceDisplayStateClock){
		RtcSetTime(desiredTimeTrackers[deviceDisplayState]);
	}
    DeviceDisplayStateLedNormal();
}
void ButtonEditFunction(uint8_t index)
{
	if (displayState == DisplayStateEdit)
		return;
		
    displayState = DisplayStateEdit;
    DeviceDisplayStateLedEdit();
	desiredTimeTrackers[deviceDisplayState] = (*ptrTimeTrackers)[deviceDisplayState];
	SevSegRefresh();
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

void SevSegRefresh(void)
{
    if (deviceDisplayState == DeviceDisplayStateTemperature) {
        SevSegSetFloatVal(temperature);
    } else {
		if (displayState == DisplayStateNormal){
			SevSegSetTimeVal(ptrTimeTrackers[deviceDisplayState]->hours, ptrTimeTrackers[deviceDisplayState]->minutes);
		}
		else if (displayState == DisplayStateEdit){
			SevSegSetTimeVal(desiredTimeTrackers[deviceDisplayState].hours, desiredTimeTrackers[deviceDisplayState].minutes);
		}
    }
}

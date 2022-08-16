// ./avrdude.exe -p atmega32 -P usb -c usbasp -U flash:w:../Clock_V1/Clock_V1/Debug/Clock_V1.hex:i


#include "main.h"
#include "Adc.h"
#include "Button.h"
#include "Led.h"
#include "Rtc.h"
#include "SevSeg.h"
#include "TimerCfg.h"
#include "TimerSw.h"
#include "Ds18b20.h"

DeviceState deviceState = DeviceStateStartup;
DeviceDisplayState deviceDisplayState = DeviceDisplayStateClock; 
//DisplayState displayState = DisplayStateOff; 
DisplayState displayState = DisplayStateNormal;
EditState editState = EditStateHoursTens;

static TimerSwHandle timerSwHandle;
const AdcValue* adcValue = NULL;

Time* ptrTimeTrackers[4] = {NULL, NULL, NULL, NULL};
Time oldTime = {};

Time desiredTimeTrackers[] = {
	[DeviceDisplayStateClock] = { (1<<4)|2, 0 },
	[DeviceDisplayStateTimer] = { 0, 0 },
	[DeviceDisplayStateAlarm] = { (1<<4)|2, 0 },
	[DeviceDisplayStateCountdown] = { 0, 5 }
};

uint8_t temperature = 25;
uint8_t oldTemperature = 0;

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
        RtcReadTime();

        //err = TimerSwIsExpired(&timerSwHandle);
        //if (err == StatusErrTime) {
            //TimerSwStartup(&timerSwHandle, 1000);
        //}
		
		SevSegRefresh();

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
		SevSegRefresh();
        DeviceDisplayStateLedNormal();
    } else {
        displayState = DisplayStateOff;
        LedAllOff();
    }
	LedAllOff();
	LedOn(index);
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
       { editState = EditStateHoursTens;}
    else {
	   editState++;
    }
}

void ButtonIncreaseFunction(uint8_t index)
{  
	if (displayState != DisplayStateEdit)
		return;

	static void (*ptrIncreaseFunction[])(Time*) = {
		[EditStateHoursTens] =     RtcHoursTensIncrease,
		[EditStateHoursUnits] =    RtcHoursUnitsIncrease,
		[EditStateMinutesTens] =   RtcMinutesTensIncrease,
		[EditStateMinutesUnits] =  RtcMinutesUnitsIncrease,
	};

    if (deviceDisplayState == DeviceDisplayStateTemperature) {
        temperature++;
    } else {
		ptrIncreaseFunction[editState](&desiredTimeTrackers[deviceDisplayState]);
    }
    SevSegRefresh();
}

void ButtonDecreaseFunction(uint8_t index)
{
	if (displayState != DisplayStateEdit)
		return;
		
	static void (*ptrDecreaseFunction[])(Time*) = {
		[EditStateHoursTens] =     RtcHoursTensDecrease,
		[EditStateHoursUnits] =    RtcHoursUnitsDecrease,
		[EditStateMinutesTens] =   RtcMinutesTensDecrease,
		[EditStateMinutesUnits] =  RtcMinutesUnitsDecrease,
	};

    if (deviceDisplayState == DeviceDisplayStateTemperature) {
        temperature--;
    } else {
		ptrDecreaseFunction[editState](&desiredTimeTrackers[deviceDisplayState]);
    }
    SevSegRefresh();
}

void ButtonOkFunction(uint8_t index)
{	if(displayState!=DisplayStateEdit)
		return;
		
    displayState = DisplayStateNormal;
	if(deviceDisplayState==DeviceDisplayStateClock){
		RtcSetTime(desiredTimeTrackers[deviceDisplayState]);
	}
    DeviceDisplayStateLedNormal();
	SevSegRefresh();
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
		if(oldTemperature!=temperature)
		{
			oldTemperature = temperature;
			SevSegSetFloatVal(temperature);
		}
		} else {
		if (displayState == DisplayStateNormal){
			if (oldTime.hours != (*ptrTimeTrackers)[deviceDisplayState].hours && oldTime.minutes != (*ptrTimeTrackers)[deviceDisplayState].minutes){
				oldTime = (*ptrTimeTrackers)[deviceDisplayState];
				SevSegSetTimeVal(ptrTimeTrackers[deviceDisplayState]->hours, ptrTimeTrackers[deviceDisplayState]->minutes);
			}
		}
		else if (displayState == DisplayStateEdit){
			SevSegSetTimeVal(desiredTimeTrackers[deviceDisplayState].hours, desiredTimeTrackers[deviceDisplayState].minutes);
		}
	}
}

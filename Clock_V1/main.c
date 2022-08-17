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
		if(deviceDisplayState == DeviceDisplayStateTimer){
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
	SevSegRefresh(false);
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
	SevSegRefresh(false);
}

void ButtonOkFunction(uint8_t index)
{
	if(displayState==DisplayStateEdit){
		displayState = DisplayStateNormal;
		if(deviceDisplayState==DeviceDisplayStateClock){
			RtcSetTime(desiredTimeTrackers[deviceDisplayState]);
		}
		
		DeviceDisplayStateLedNormal();
		SevSegRefresh(false);
	}
	else if (deviceDisplayState == DeviceDisplayStateTimer){
		RtcTimerToggle();
		SevSegRefresh(false);
	}
}

void ButtonEditFunction(uint8_t index)
{
	if (deviceDisplayState == DeviceDisplayStateTimer){
		RtcTimerRestart();
		SevSegRefresh(false);
	}else
	if (displayState == DisplayStateNormal){
		
		displayState = DisplayStateEdit;
		DeviceDisplayStateLedEdit();
		if(deviceDisplayState == DeviceDisplayStateTemperature){
			oldTemperature = temperature;
			}else{
			desiredTimeTrackers[deviceDisplayState] = (*ptrTimeTrackers)[deviceDisplayState];
		}
		SevSegRefresh(false);
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
	if (deviceDisplayState == DeviceDisplayStateTemperature) {
		if(oldTemperature!=temperature || !optional)
		{
			oldTemperature = temperature;
			SevSegSetTemperatureVal(temperature);
		}
	}
	else {
		if (displayState == DisplayStateNormal){
			if (oldTime.hours != (*ptrTimeTrackers)[deviceDisplayState].hours
			|| oldTime.minutes != (*ptrTimeTrackers)[deviceDisplayState].minutes
			|| oldTime.seconds != (*ptrTimeTrackers)[deviceDisplayState].seconds
			|| !optional){
				oldTime = (*ptrTimeTrackers)[deviceDisplayState];
				if(deviceDisplayState==DeviceDisplayStateTimer && !(ptrTimeTrackers[deviceDisplayState]->hours))
				{
					SevSegSetTimeMinutesVal(ptrTimeTrackers[deviceDisplayState]->minutes,
					ptrTimeTrackers[deviceDisplayState]->seconds);
				}
				else
				{
					SevSegSetTimeHoursVal(ptrTimeTrackers[deviceDisplayState]->hours, ptrTimeTrackers[deviceDisplayState]->minutes);
				}
			}
		}
		else if (displayState == DisplayStateEdit){
			SevSegSetTimeHoursVal(desiredTimeTrackers[deviceDisplayState].hours, desiredTimeTrackers[deviceDisplayState].minutes);
		}
	}
}

void BuzzerFunction(void){
    //call buzzer
}

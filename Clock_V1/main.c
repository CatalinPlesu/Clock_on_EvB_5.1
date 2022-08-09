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
DeviceDisplayState deviceDisplayState = DeviceDisplayStateTemperature;
/* DeviceDisplayState deviceDisplayState = DeviceDisplayStateClock; */
/* DisplayState displayState = DisplayStateOff; */
DisplayState displayState = DisplayStateNormal;
EditState editState = EditStateHours;

static TimerSwHandle timerSwHandle;
const AdcValue* adcValue = NULL;

const TimeBCD* ptrTimeBcd = NULL;

Time timeTrackers[] = {
    [DeviceDisplayStateClock] = { 12, 42 },
    [DeviceDisplayStateTimer] = { 01, 20 },
    [DeviceDisplayStateAlarm] = { 13, 30 },
    [DeviceDisplayStateCountdown] = { 1, 30 }
};

uint16_t temperature = 27;

void ButtonPowerFunction(uint8_t index);
void ButtonNextFunction(uint8_t index);
void ButtonToggleFunction(uint8_t index);
void ButtonIncreaseFunction(uint8_t index);
void ButtonDecreaseFunction(uint8_t index);
void ButtonOkFunction(uint8_t index);
void ButtonEditFunction(uint8_t index);

void SevSegRefresh(void);

static ButtonFunctionPtr buttonFunctionPtr = {
    ButtonPowerFunction,
    ButtonNextFunction,
    ButtonToggleFunction,
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
        ptrTimeBcd = GetRtcTime();

        sei();

        err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);
        if (err == StatusErrNone) {
            TimerSwStartup(&timerSwHandle, 1000);
        }
    }

    TimeBCD desiredTimeBcd = {};
    desiredTimeBcd.minutes.bits.tens = 2;
    desiredTimeBcd.minutes.bits.units = 5;
    desiredTimeBcd.hours.bits.tens = 1;
    desiredTimeBcd.hours.bits.units = 4;
    RtcSetTime(desiredTimeBcd);

    while (1) {
        ButtonRoutine();

        err = TimerSwIsExpired(&timerSwHandle);
        if (err == StatusErrTime && !(deviceDisplayState == DeviceDisplayStateClock && displayState == DisplayStateEdit)) {

            TimeIncrement(&timeTrackers[DeviceDisplayStateClock]);

            SevSegRefresh();

            TimerSwStartup(&timerSwHandle, 1000);
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

void ButtonToggleFunction(uint8_t index)
{
    if (displayState != DisplayStateEdit)
        return;

    if (editState == EditStateHours)
        editState = EditStateMinutes;
    else
        editState = EditStateHours;
}

void ButtonIncreaseFunction(uint8_t index)
{
    if (displayState != DisplayStateEdit)
        return;

    if (deviceDisplayState == DeviceDisplayStateTemperature) {
        temperature++;
    } else {
        if (editState == EditStateHours) {
            timeTrackers[deviceDisplayState ].hours++;
        } else {
            timeTrackers[deviceDisplayState ].minutes++;
        }
        TimeValidate(&timeTrackers[deviceDisplayState ]);
    }
    SevSegRefresh();
}

void ButtonDecreaseFunction(uint8_t index)
{
    if (displayState != DisplayStateEdit)
        return;

    if (deviceDisplayState == DeviceDisplayStateTemperature) {
        temperature--;
    } else {
        if (editState == EditStateHours) {
            timeTrackers[deviceDisplayState ].hours--;
        } else {
            timeTrackers[deviceDisplayState ].minutes--;
        }
        TimeValidate(&timeTrackers[deviceDisplayState ]);
    }
    SevSegRefresh();
}

void ButtonOkFunction(uint8_t index)
{
    displayState = DisplayStateNormal;
    DeviceDisplayStateLedNormal();
}
void ButtonEditFunction(uint8_t index)
{
    displayState = DisplayStateEdit;
    DeviceDisplayStateLedEdit();
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
        /* SevSegSetFloatVal(temperature); */
        /* SevSegSetFloatVal(ptrTimeBcd->minutes.bits.units); */
        /* SevSegSetTimeBcd(*ptrTimeBcd); */
        SevSegSetTimeBcd(*ptrTimeBcd);
    } else {
        SevSegSetTimeVal(timeTrackers[deviceDisplayState]);
    }
}

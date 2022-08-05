// ./avrdude.exe -p atmega32 -P usb -c usbasp -U flash:w:../Clock_V1/Clock_V1/Debug/Clock_V1.hex:i

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "config.h"
#include "Adc.h"
#include "Rtc.h"
#include "SevSeg.h"
#include "TimerCfg.h"
#include "TimerSw.h"
#include "Button.h"
#include "Led.h"

DeviceState deviceState = DeviceStateStartup;
DeviceDisplayState deviceDisplayState = DeviceDisplayStateClock;
DisplayState displayState = DisplayStateOff;
EditState editState = EditStateHours; 

static TimerSwHandle timerSwHandle;
const AdcValue* adcValue = NULL;

Time timeTrackers[] = {
	[DeviceDisplayStateClock] = {12, 53},
	[DeviceDisplayStateTimer] = {},
	[DeviceDisplayStateAlarm] = {13, 30},
	[DeviceDisplayStateContdown] = {1, 30}
};

uint16_t temperature = 27;

void ButtonPowerFunction(uint8_t index);
void ButtonNextFunction(uint8_t index);
void ButtonToggleFunction(uint8_t index);
void ButtonIncreaseFunction(uint8_t index);
void ButtonDecreaseFunction(uint8_t index);
void ButtonOkFunction(uint8_t index);
void ButtonEditFunction(uint8_t index);

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

	if (deviceState == DeviceStateStartup){
		deviceState = DeviceStateInit;

		TimerInitCfg();
		TimerEnableCfg(true);

		SevSegInit();
		ButtonInit(&buttonFunctionPtr);
		LedInit();

		AdcInit();
		adcValue = GetAdcValue();

		sei();

		err = TimerSwInit(pTimerSwInitParam, &timerSwHandle);
		if (err == StatusErrNone) {
			TimerSwStartup(&timerSwHandle, 1000);
		}
	}
	
    while (1) {
        ButtonRoutine();
        
        err = TimerSwIsExpired(&timerSwHandle);
        if (err == StatusErrTime && !(deviceDisplayState==DeviceDisplayStateClock&&displayState==DisplayStateEdit)) {
			
			TimeIncrement(&timeTrackers[DeviceDisplayStateClock]);
			
			for(DeviceDisplayState state = DeviceDisplayStateClock; state <= DeviceDisplayStateTemperature; state++)
			{
				if (deviceDisplayState == state){
					if (state == DeviceDisplayStateTemperature)
						{SevSegSetFloatVal(temperature);}
					else
						{SevSegSetTimeVal(timeTrackers[state]);}
					}
			}
						
            TimerSwStartup(&timerSwHandle, 1000);
        }
		
		if(deviceDisplayState==DeviceDisplayStateTemperature)
			SevSegSetFloatVal(temperature);
			
		if(displayState==DisplayStateOff)
			SevSegCfgAllDigitsOff();
		else
			SevSegRutine();

    }
}

void ButtonPowerFunction(uint8_t index){
	if(displayState==DisplayStateOff){
		displayState = DisplayStateNormal;
		DeviceDisplayStateLedNormal();
	}
	else{
		displayState = DisplayStateOff;
		LedAllOff();
		}
}

void ButtonNextFunction(uint8_t index){
	if (displayState!=DisplayStateNormal)
		return;
		
	if(deviceDisplayState==DeviceDisplayStateTemperature)
		deviceDisplayState=0;
	else
		deviceDisplayState++;
	DeviceDisplayStateLedNormal();
}

void ButtonToggleFunction(uint8_t index){
	if (displayState!=DisplayStateEdit)
		return;
		
	if(editState==EditStateHours)
		editState = EditStateMinutes;
	else
		editState = EditStateHours;
}

void ButtonIncreaseFunction(uint8_t index){
	if (displayState!=DisplayStateEdit)
		return;
		
	for(DeviceDisplayState state = DeviceDisplayStateClock; state <= DeviceDisplayStateTemperature; state++)
	{
		if (deviceDisplayState == state){
			if (state == DeviceDisplayStateTemperature){
				temperature++;
				SevSegSetFloatVal(temperature);
			}
			else{
				if(editState==EditStateHours)
					{
						timeTrackers[state].hours++;
					}
				else
					{timeTrackers[state].minutes++;}
				TimeValidate(&timeTrackers[state]);
				SevSegSetTimeVal(timeTrackers[state]);
			}
		}
	}
}

void ButtonDecreaseFunction(uint8_t index){
	if (displayState!=DisplayStateEdit)
		return;
		
	for(DeviceDisplayState state = DeviceDisplayStateClock; state <= DeviceDisplayStateTemperature; state++)
	{
		if (deviceDisplayState == state){
		if (state == DeviceDisplayStateTemperature){
			temperature--;
			SevSegSetFloatVal(temperature);
		}
		else{
			if(editState==EditStateHours)
				{timeTrackers[state].hours--;}
			else
				{timeTrackers[state].minutes--;}
			TimeValidate(&timeTrackers[state]);
			SevSegSetTimeVal(timeTrackers[state]);
		}
		}
	}
}

void ButtonOkFunction(uint8_t index){
	displayState = DisplayStateNormal;
	DeviceDisplayStateLedNormal();
}
void ButtonEditFunction(uint8_t index){
	displayState = DisplayStateEdit;
	DeviceDisplayStateLedEdit();
}


void DeviceDisplayStateLedNormal(void){
	LedAllOff();
	LedOn(deviceDisplayState);
}

void DeviceDisplayStateLedEdit(void){
	LedAllOn();
	LedOff(deviceDisplayState);
}
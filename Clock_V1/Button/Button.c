#include "Button.h"
#include "Led.h"
#include "SevSeg.h"
#include "SevSegCfg.h"
#include "TimerCfg.h"

static ButtonHandleConfig buttonHandleConfig;

static TogglState togglState[BUTTON_COUNT] = {};
static ButtonState buttonState[BUTTON_COUNT] = {};
static uint8_t counter[BUTTON_COUNT] = {};
static TimerSwHandle timerSwHandle[BUTTON_COUNT] = {};
StatusError err;

ButtonFunctionPtr* buttonFunctionPtr;

void ButtonInit(ButtonFunctionPtr* _buttonFuctionPtr){
	buttonFunctionPtr = _buttonFuctionPtr;

	buttonHandleConfig.pinValue[0] = PIN_BUTTON_POWER;
	buttonHandleConfig.pinValue[1] = PIN_BUTTON_NEXT;
	buttonHandleConfig.pinValue[2] = PIN_BUTTON_TOGGLE;
	buttonHandleConfig.pinValue[3] = PIN_BUTTON_INCREASE;
	buttonHandleConfig.pinValue[4] = PIN_BUTTON_DECREASE;
	buttonHandleConfig.pinValue[5] = PIN_BUTTON_OK;
	buttonHandleConfig.pinValue[6] = PIN_BUTTON_ABORT;
	buttonHandleConfig.count = BUTTON_COUNT;

	// makes button pins input
	for (uint8_t i = 0; i < buttonHandleConfig.count; i++)
		PORT_DIRECTION_BUTTON &= ~(1 << buttonHandleConfig.pinValue[i]);

	// enables pull up resistor for all buttons
	for(uint8_t i = 0; i < buttonHandleConfig.count; i++)
		PORT_BUTTON |= (1 << buttonHandleConfig.pinValue[i]);
		
	TimerSwInitParam *pTimerSwInitParam = TimerGetIntervalPointerCfg();
	
	for(uint8_t i = 0; i < buttonHandleConfig.count; i++){
		err = TimerSwInit(pTimerSwInitParam,&timerSwHandle[i]);
		if (err == StatusErrNone)
		{
			TimerSwStartup(&timerSwHandle[i],BUTTON_TIMER_MS);
		}
	}
}

void ButtonRoutine(void)
{
	for (uint8_t i = 0; i < buttonHandleConfig.count; i++) {
			if (PIN_BUTTON & (1 << buttonHandleConfig.pinValue[i])) { // not pressed
				if (counter[i] != 0)
					counter[i]--;
				 else
					buttonState[i] = ButtonStateRealesed;
				} 
				else { // pressed
				if (counter[i] <= MAX_COUNTER_VALUE) 
					counter[i]++;
				else
					buttonState[i] = ButtonStatePressed;
			}

			if (buttonState[i] == ButtonStateRealesed) 
				togglState[i] = TogglStateNo;
			else if (togglState[i] == TogglStateNo){
				togglState[i] = TogglStateWaiting;
				TimerSwStartup(&timerSwHandle[i],BUTTON_TIMER_MS);
			}

			if (togglState[i] == TogglStateWaiting) {
				//err = TimerSwIsExpired(&timerSwHandle[i]);
				//if (err == StatusErrTime)
				//{
					//(*buttonFunctionPtr)[i](1);
					//TimerSwStartup(&timerSwHandle[i],BUTTON_TIMER_MS);
					//togglState[i] = TogglStateDone;
				//}
				//else if (buttonState[i] == ButtonStateRealesed){
					//(*buttonFunctionPtr)[i](0);
					//togglState[i] = TogglStateDone;
				//}
				(*buttonFunctionPtr)[i](i);
				togglState[i] = TogglStateDone;
			}
	}
}
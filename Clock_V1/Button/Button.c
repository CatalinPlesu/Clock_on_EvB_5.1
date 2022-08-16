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
	
	ButtonHandleConfig button_handle_config = {
		.ddr = {&BUTTON0_DDR, &BUTTON1_DDR, &BUTTON2_DDR, &BUTTON3_DDR,
			&BUTTON4_DDR, &BUTTON5_DDR, &BUTTON7_DDR},
		.port = {&BUTTON0_PORT, &BUTTON1_PORT, &BUTTON2_PORT, &BUTTON3_PORT,
			&BUTTON4_PORT, &BUTTON5_PORT, &BUTTON7_PORT},
		.pin = {&BUTTON0_PIN, &BUTTON1_PIN, &BUTTON2_PIN,
			&BUTTON3_PIN, &BUTTON4_PIN, &BUTTON5_PIN, &BUTTON7_PIN},
		.pinValue = {BUTTON0_PIN_VALUE, BUTTON1_PIN_VALUE, BUTTON2_PIN_VALUE, 
			BUTTON3_PIN_VALUE, BUTTON4_PIN_VALUE, BUTTON5_PIN_VALUE, BUTTON7_PIN_VALUE},
		.count = BUTTON_COUNT,
	};
	buttonHandleConfig = button_handle_config;

	// makes button pins input
	for (uint8_t i = 0; i < buttonHandleConfig.count; i++){
	    *buttonHandleConfig.ddr[i] &= ~(0x01 << buttonHandleConfig.pinValue[i]);
	}

	// enables pull up resistor for all buttons
	for(uint8_t i = 0; i < buttonHandleConfig.count; i++)
	    *buttonHandleConfig.port[i] |= (0x1 << buttonHandleConfig.pinValue[i]);
	
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
		err = TimerSwIsExpired(&timerSwHandle[i]);
		if (err == StatusErrTime)
		{
			if (*buttonHandleConfig.pin[i] & (1 << buttonHandleConfig.pinValue[i])) { // not pressed
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
			}

			if (togglState[i] == TogglStateWaiting) {
				(*buttonFunctionPtr)[i](i);
				togglState[i] = TogglStateDone;
				buttonState[i] = ButtonStateUnpressed;
			}
			TimerSwStartup(&timerSwHandle[i],BUTTON_TIMER_MS);
		}
	}
}

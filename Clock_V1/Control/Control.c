#include "Control.h"

static ControlHandleConfig* controlHandleConfig;
TogglState togglState[BUTTON_COUNT] = {};
ButtonState buttonState[BUTTON_COUNT] = {};
static uint16_t counter[BUTTON_COUNT] = {};
uint8_t *active = (void*)0;

void ControlInit(uint8_t* a)
{
	active = a;
    controlHandleConfig = ControlCfgInitAndGet();
    ControlCfgAllLedsOff();
}

void ControlRoutine(void)
{
    for (uint8_t i = 0; i < controlHandleConfig->buttonCount; i++) {

        if (*controlHandleConfig->pinButton & (1 << controlHandleConfig->buttonPinValue[i])) {
            if (counter[i] != 0) {
                counter[i]--;
            } else 
			{
                buttonState[i] = ButtonStateRealesed;
            }
        } else {
            if (counter[i] <= MAX_COUNTER_VALUE) {
                counter[i]++;
            } 
			else 
			{
                buttonState[i] = ButtonStatePressed;
            }
        }

        if (buttonState[i] == ButtonStateRealesed) {
            togglState[i] = TogglStateNo;
        } else {
            if (togglState[i] == TogglStateNo) {
                togglState[i] = TogglStateWaiting;
            }
        }

        if (togglState[i] == TogglStateWaiting) {
			if(i < controlHandleConfig->ledCount)
			    *controlHandleConfig->portLed ^= (1 << controlHandleConfig->ledPinValue[i]);
				if(i == 6)
					*active=*active==0?1:0;
				
            togglState[i] = TogglStateDone;
        }
    }
}


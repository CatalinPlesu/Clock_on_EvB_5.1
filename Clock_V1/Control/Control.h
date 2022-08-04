#ifndef CONTROL_H_
#define CONTROL_H_

#include "ControlCfg.h"

#define MAX_COUNTER_VALUE 100

typedef enum
{
	TogglStateNo,
	TogglStateWaiting,
	TogglStateDone
}TogglState;

typedef enum
{
	ButtonStatePressed,
	ButtonStateRealesed,
}ButtonState;

void ControlInit(uint8_t* a);

void ControlRoutine(void);

#endif

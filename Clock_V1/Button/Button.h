#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>
#include "ButtonCfg.h"

typedef enum
{
	ButtonPower,
	ButtonNext,
	ButtonNextDigit,
	ButtonIncrease,
	ButtonDecrease,
	ButtonOk,
	ButtonEdit
}Button;

typedef enum
{
	TogglStateNo,
	TogglStateWaiting,
	TogglStateDone
}TogglState;

typedef enum
{
	ButtonStateUnpressed,
	ButtonStateRealesed,
	ButtonStatePressed,
}ButtonState;

typedef struct
{
	volatile uint8_t *ddr[BUTTON_COUNT];
	volatile uint8_t *port[BUTTON_COUNT];
	volatile uint8_t *pin[BUTTON_COUNT];
	uint8_t pinValue[BUTTON_COUNT];
	uint8_t count;
}ButtonHandleConfig;

// index 1:3 - number of clicks
typedef void (*ButtonFunctionPtr[BUTTON_COUNT])(uint8_t index);

void ButtonInit(ButtonFunctionPtr* _buttonFuctionPtr);
void ButtonRoutine(void);

#endif /* BUTTON_H_ */
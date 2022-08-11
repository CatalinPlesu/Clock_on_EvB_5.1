#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>

#define PORT_DIRECTION_BUTTON DDRD
#define PORT_BUTTON PORTD
#define PIN_BUTTON PIND
#define PIN_BUTTON_POWER PD0
#define PIN_BUTTON_NEXT PD1
#define PIN_BUTTON_TOGGLE PD2
#define PIN_BUTTON_INCREASE PD3
#define PIN_BUTTON_DECREASE PD4
#define PIN_BUTTON_OK PD5
#define PIN_BUTTON_ABORT PD6
#define BUTTON_COUNT 7

#define MAX_COUNTER_VALUE 100
#define BUTTON_TIMER_MS 1000

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
	ButtonStatePressed,
	ButtonStateRealesed,
}ButtonState;

typedef struct
{
	uint8_t count;
	uint8_t pinValue[BUTTON_COUNT];
}ButtonHandleConfig;

typedef void (*ButtonFunctionPtr[BUTTON_COUNT])(uint8_t index);

void ButtonInit(ButtonFunctionPtr* _buttonFuctionPtr);
void ButtonRoutine(void);

#endif /* BUTTON_H_ */
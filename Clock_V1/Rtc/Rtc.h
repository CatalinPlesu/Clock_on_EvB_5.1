#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>
#include "RtcCfg.h"

typedef enum
{
	RtcTwiStateIdle,
	RtcTwiStateStart,
	RtcTwiStateRestart,
	RtcTwiStateStop,
	RtcTwiStateError,
}RtcTwiState;

typedef enum{
    RtcTwiTaskWrite,
    RtcTwiTaskRead,
}RtcTwiTask;

typedef enum{
    RtcTwiTaskDataWrite,
    RtcTwiTaskDataRead,
}RtcTwiTaskData;

typedef enum{
    RtcTwiDataSla,
    RtcTwiDataReg,
    RtcTwiDataMinutes,
    RtcTwiDataHours,
}RtcTwiData;

struct MinutesBCD{
    uint8_t unused:1;
    uint8_t tens:3;
    uint8_t units:4;
};

typedef union{
    uint8_t byte;
    struct MinutesBCD bits;
}Minutes;

struct HoursBCD{
    uint8_t unused:2;
    uint8_t tens:2;
    uint8_t units:4;
};

typedef union{
    uint8_t byte;
    struct HoursBCD bits;
}Hours;

typedef struct{
    uint8_t hours;
    uint8_t minutes;
}Time;

typedef struct{
    Minutes minutes;
    Hours hours;
}TimeBCD;

void RtcInit(void);
TimeBCD* GetRtcTime(void);

void RtcSla(void);
void RtcRegister(void);
void RtcMinutes(void);
void RtcHours(void);

void RtcSetTime(TimeBCD time);

void TimeIncrement(Time* time);
void TimeDecrement(Time* time);
void TimeValidate(Time* time);

#endif

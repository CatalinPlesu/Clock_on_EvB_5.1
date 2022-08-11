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

typedef struct{
    uint8_t hours;
    uint8_t minutes;
}Time;

void RtcInit(void);
void RtcReadTime(void);
void RtcWriteTime(void);
Time* GetRtcTime(void);

void RtcSla(void);
void RtcRegister(void);
void RtcMinutes(void);
void RtcHours(void);

void RtcSetTime(Time time);
Time RtcCreateTime(uint8_t hours, uint8_t minutes);

void TimeIncrement(Time* time);
void TimeDecrement(Time* time);
void TimeValidate(Time* time);

#endif

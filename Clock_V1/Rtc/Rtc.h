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
Time* GetRtcTimer(void);
Time* GetRtcAlarm(void);
Time* GetRtcCountdown(void);

void RtcSetTime(Time time);
Time RtcCreateTime(uint8_t hours, uint8_t minutes);

void RtcHoursTensIncrease(Time* time);
void RtcHoursUnitsIncrease(Time* time);
void RtcMinutesTensIncrease(Time* time);
void RtcMinutesUnitsIncrease(Time* time);
void RtcHoursTensDecrease(Time* time);
void RtcHoursUnitsDecrease(Time* time);
void RtcMinutesTensDecrease(Time* time);
void RtcMinutesUnitsDecrease(Time* time);


#endif

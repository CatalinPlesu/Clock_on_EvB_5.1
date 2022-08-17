#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>
#include "RtcCfg.h"

typedef enum{
	RtcTimerStateIdle,
	RtcTimerStateRunning,
}RtcTimerState;

typedef struct{
	uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
}Time;

Time* GetRtcTime(void);
Time* GetRtcTimer(void);
Time* GetRtcAlarm(void);
Time* GetRtcCountdown(void);

void RtcInit(void);
void RtcReadTime(void);
void RtcWriteTime(void);
void RtcSetTime(Time time);
Time RtcCreateTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

void RtcTimerToggle(void);
void RtcTimerRestart(void);
void RtcTimerRoutine(void);

void RtcHoursTensIncrease(Time* time);
void RtcHoursUnitsIncrease(Time* time);
void RtcMinutesTensIncrease(Time* time);
void RtcMinutesUnitsIncrease(Time* time);
void RtcHoursTensDecrease(Time* time);
void RtcHoursUnitsDecrease(Time* time);
void RtcMinutesTensDecrease(Time* time);
void RtcMinutesUnitsDecrease(Time* time);


#endif

#ifndef RTC_H_
#define RTC_H_

#include "RtcCfg.h"
#include <stdint.h>

void RtcInit(void (*ptrAlarmFunction)(void));

Time* GetRtcTime(void);
Time* GetRtcTimer(void);
Time* GetRtcAlarm(void);
Time* GetRtcCountdown(void);

void RtcReadTime(void);
void RtcSetTime(Time desiredTime);
Time RtcCreateTime(int8_t hours, int8_t minutes, int8_t seconds);

void RtcTimerToggle(void);
void RtcTimerRestart(void);
void RtcTimerRoutine(void);

void RtcAlarmToggle(void);

void RtcHoursTensIncrease(Time* time);
void RtcHoursUnitsIncrease(Time* time);
void RtcMinutesTensIncrease(Time* time);
void RtcMinutesUnitsIncrease(Time* time);
void RtcHoursTensDecrease(Time* time);
void RtcHoursUnitsDecrease(Time* time);
void RtcMinutesTensDecrease(Time* time);
void RtcMinutesUnitsDecrease(Time* time);

#endif

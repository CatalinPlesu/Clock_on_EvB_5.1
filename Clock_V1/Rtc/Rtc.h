#ifndef RTC_H_
#define RTC_H_

#include "RtcCfg.h"
#include "Led.h"
#include <stdint.h>
#include <stdbool.h>

void RtcInit(void);

RtcDisplayData RtcExtractTime(Time timeTracker, bool isTimer, uint8_t editIndex);

Time* GetRtcTime(void);
Time* GetRtcTimer(void);
Time* GetRtcAlarm(void);
Time* GetRtcCountdown(void);

void RtcTimeTick(void);
void RtcReadTime(void);
void RtcSetTime(Time desiredTime);
Time RtcCreateTime(int8_t hours, int8_t minutes, int8_t seconds);

uint8_t hours_to_uint8_t(uint8_t hours);
uint8_t minutes_to_uint8_t(uint8_t minutes);
uint8_t seconds_to_uint8_t(uint8_t seconds);

void RtcTimerToggle(void);
void RtcTimerRestart(void);
void RtcTimerRoutine(void);

void RtcAlarmToggle(void);
void RtcAlarmIndicator(void);
void RtcAlarmSet(Time desiredAlarm);

void RtcCountdownSet(Time desiredCountdown);
void RtcCountdownToggle();
void RtcCountdownRoutine(void);

#endif

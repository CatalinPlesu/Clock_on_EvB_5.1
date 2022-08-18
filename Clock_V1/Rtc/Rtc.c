#include <avr/interrupt.h>
#include <stddef.h>

#include "Rtc.h"

PCF8563ConfigHandle* pcf8563ConfigHandle = NULL;
static Time time = {};

static Time timerStarted = { 0, 0 };
static Time timer = { 0, 0 };
static RtcTimerState rtcTimerState = RtcTimerStateIdle;

static Time alarm = { 1 << 4 | 2, 0 };
static RtcAlarmInterrupt rtcAlarmInterrupt = RtcAlarmInterruptDisabled;
static Time countdown = { 3 << 4 , 0, 5 };

void RtcInit(void)
{
	pcf8563ConfigHandle = RtcCfgInitAndGet();
	alarm.minutes = pcf8563ConfigHandle->minute_alarm;
	alarm.hours = pcf8563ConfigHandle->hour_alarm;

	if (pcf8563ConfigHandle->control_status_2 & (0x01 << AIE)) {
		rtcAlarmInterrupt = RtcAlarmInterruptEnabled;
		} else {
		rtcAlarmInterrupt = RtcAlarmInterruptDisabled;
	}
	RtcReadTime();
	RtcTimerRestart();
}

RtcDisplayData RtcExtractTime(Time timeTracker, bool isTimer, uint8_t editIndex){
	timeTracker.hours &= ~(0x01<<7);
	timeTracker.minutes &= ~(0x01<<7);
	RtcDisplayData rtcDisplayData  = {};
	uint8_t digits[6] = {
		HOURS_MASK_TENS(timeTracker.hours),
		HOURS_MASK_UNITS(timeTracker.hours),
		MINUTES_MASK_TENS(timeTracker.minutes),
		MINUTES_MASK_UNITS(timeTracker.minutes),
		SECONDS_MASK_TENS(timeTracker.seconds),
		SECONDS_MASK_UNITS(timeTracker.seconds),
	};
	
	uint8_t j = 0;
	uint8_t blink_position = editIndex;
	
	if (editIndex == 5){
		blink_position = 3;
		editIndex = 2;
	}
	else if (editIndex>2){
		blink_position = 2;
		editIndex -= 2;
		}else{
		editIndex = 0;
	}
	rtcDisplayData.dots|=(0x01<<(4+blink_position)); // will enable blinking
	
	if (isTimer && !timeTracker.hours){
		editIndex = 2;
	}
	
	for(uint8_t i = editIndex; i < editIndex + 4; i++){
		rtcDisplayData.digit[j] = digits[i];
		if(i & 0x01  && j!=3){
			rtcDisplayData.dots |= (0x01 << j);
		}
		j++;
	}
	return rtcDisplayData;
}

Time* GetRtcTime(void)
{
	return &time;
}
Time* GetRtcTimer(void)
{
	return &timer;
}
Time* GetRtcAlarm(void)
{
	return &alarm;
}
Time* GetRtcCountdown(void)
{
	return &countdown;
}

void RtcTimeTick(void){
	time = RtcCreateTime(hours_to_uint8_t(time.hours),
		minutes_to_uint8_t(time.minutes),
		seconds_to_uint8_t(time.seconds) + 1);
}

void RtcReadTime(void)
{
	time = RtcCfgReadTime();
}

void RtcSetTime(Time desiredTime)
{
	time = desiredTime;
	RtcCfgWriteTime(desiredTime);
}

Time RtcCreateTime(int8_t hours, int8_t minutes, int8_t seconds)
{
	if (seconds < 0) {
		minutes -= 1;
		seconds += 60;
		}else if (seconds >59){
		seconds-=60;
		minutes+=1;
	}
	if (minutes < 0) {
		hours -= 1;
		minutes += 60;
		}else if(minutes >= 59){
		minutes-=60;
		hours+=1;
	}
	if (hours < 0) {
		hours = 23;
		} else if(hours>23){
		hours=0;
	}

	Time time = {};
	time.hours = ((hours / 10) << 4) | (hours % 10);
	time.minutes = ((minutes / 10) << 4) | (minutes % 10);
	time.seconds = ((seconds / 10) << 4) | (seconds % 10);
	return time;
}

uint8_t hours_to_uint8_t(uint8_t hours)
{
	return (HOURS_MASK_TENS(hours) * 10 + HOURS_MASK_UNITS(hours));
}

uint8_t minutes_to_uint8_t(uint8_t minutes)
{
	return (MINUTES_MASK_TENS(minutes) * 10 + MINUTES_MASK_UNITS(minutes));
}

uint8_t seconds_to_uint8_t(uint8_t seconds)
{
	return (SECONDS_MASK_TENS(seconds) * 10 + SECONDS_MASK_UNITS(seconds));
}

void RtcTimerToggle(void)
{
	if (rtcTimerState == RtcTimerStateIdle) {
		rtcTimerState = RtcTimerStateRunning;
		timerStarted = RtcCreateTime(hours_to_uint8_t(time.hours) - hours_to_uint8_t(timer.hours),
			minutes_to_uint8_t(time.minutes) - minutes_to_uint8_t(timer.minutes),
			seconds_to_uint8_t(time.seconds) - seconds_to_uint8_t(timer.seconds));
		} else {
			rtcTimerState = RtcTimerStateIdle;
	}
}

void RtcTimerRestart(void)
{
	if (rtcTimerState == RtcTimerStateIdle) {
		rtcTimerState = RtcTimerStateRunning;
		} 
		else {
		rtcTimerState = RtcTimerStateIdle;
	}
	timerStarted = time;
	timer = (Time) {};
}

void RtcTimerRoutine(void)
{
	if (rtcTimerState == RtcTimerStateRunning) {
		timer = RtcCreateTime(hours_to_uint8_t(time.hours) - hours_to_uint8_t(timerStarted.hours),
			minutes_to_uint8_t(time.minutes) - minutes_to_uint8_t(timerStarted.minutes),
			seconds_to_uint8_t(time.seconds) - seconds_to_uint8_t(timerStarted.seconds));
	}
}

void RtcAlarmToggle(void)
{
	if (rtcAlarmInterrupt == RtcAlarmInterruptDisabled) {
		RtcCfgAlarmEnable();
		rtcAlarmInterrupt = RtcAlarmInterruptEnabled;
		} else {
		RtcCfgAlarmDisable();
		rtcAlarmInterrupt = RtcAlarmInterruptDisabled;
	}
}

void RtcAlarmIndicator(void){
	if(rtcAlarmInterrupt==RtcAlarmInterruptEnabled){
		LedOn(3);
	}
}

void RtcAlarmSet(Time alarm){
	RtcCfgWriteAlarm(alarm);
}
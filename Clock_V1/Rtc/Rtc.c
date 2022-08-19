#include <avr/interrupt.h>
#include <stddef.h>

#include "Rtc.h"

PCF8563ConfigHandle* pcf8563ConfigHandle = NULL;
static Time time;

static Time timerStarted;
static Time timer;
static ToggleState timerState = ToggleStateDisabled;

static Time alarm;
static ToggleState alarmState = ToggleStateDisabled;

static Time countdown;
static Time countdownFinish;
static ToggleState countdownState = ToggleStateDisabled;

static void RtcCfgCheckAlarmState(void);

void RtcInit(void)
{
	pcf8563ConfigHandle = RtcCfgInitAndGet();
	RtcCfgCheckAlarmState();
	RtcReadTime();
	RtcTimerRestart();
}

RtcDisplayData RtcExtractTime(Time timeTracker, bool isTimer, uint8_t editIndex){
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
	//minutes += (-1 * seconds < 0) + (seconds > 60)
	//seconds %= 60;
	//seconds *= (-1 * seconds < 0) + (seconds > 60)
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
		}
		else if(minutes >= 59){
		minutes-=60;
		hours+=1;
	}
	if (hours < 0) {
		hours = 23;
		}
		 else if(hours>23){
		 hours=0;
	}

	Time time;
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
	if (timerState == ToggleStateDisabled) {
		timerState = ToggleStateEnabled;
		timerStarted = RtcCreateTime(hours_to_uint8_t(time.hours) - hours_to_uint8_t(timer.hours),
			minutes_to_uint8_t(time.minutes) - minutes_to_uint8_t(timer.minutes),
			seconds_to_uint8_t(time.seconds) - seconds_to_uint8_t(timer.seconds));
		} else {
			timerState = ToggleStateDisabled;
	}
}

void RtcTimerRestart(void)
{
	if (timerState == ToggleStateDisabled) {
			timerState  = ToggleStateEnabled;
	} 
	else {
		timerState = ToggleStateDisabled;
	}
	timerStarted = time;
	timer = (Time) {};
}

void RtcTimerRoutine(void)
{
	if (timerState == ToggleStateEnabled) {
		timer = RtcCreateTime(hours_to_uint8_t(time.hours) - hours_to_uint8_t(timerStarted.hours),
			minutes_to_uint8_t(time.minutes) - minutes_to_uint8_t(timerStarted.minutes),
			seconds_to_uint8_t(time.seconds) - seconds_to_uint8_t(timerStarted.seconds));
	}
}

void RtcAlarmToggle(void)
{
	if (alarmState == ToggleStateDisabled) {
		RtcCfgAlarmEnable();
		} else 
	{
		RtcCfgAlarmDisable();
	}
	RtcCfgCheckAlarmState();
}

void RtcAlarmIndicator(void){
	if(alarmState==ToggleStateEnabled){
		LedOn(3);
	}
}

void RtcAlarmSet(Time desiredAlarm){
	RtcCfgWriteAlarm(desiredAlarm);
	alarm = desiredAlarm;
}

static void RtcCfgCheckAlarmState(void){
	if (pcf8563ConfigHandle->control_status_2 & (0x01 << AIE)) {
		alarmState = ToggleStateEnabled;
		} else {
		alarmState = ToggleStateDisabled;
	}
}


void RtcCountdownSet(Time desiredCountdown){
	countdownState = ToggleStateEnabled;
	countdownFinish = RtcCreateTime(hours_to_uint8_t(desiredCountdown.hours) + hours_to_uint8_t(timer.hours),
			minutes_to_uint8_t(desiredCountdown.minutes) + minutes_to_uint8_t(timer.minutes),
			seconds_to_uint8_t(desiredCountdown.seconds) + seconds_to_uint8_t(timer.seconds));
}

void RtcCountdownToggle(){
	if (countdownState == ToggleStateDisabled) {
		RtcCountdownSet(countdown);
		} else {
			countdownState = ToggleStateDisabled;
	}
}

void RtcCountdownRoutine(void){
	if(countdownState == ToggleStateEnabled){
		if(countdown.hours == 0 && countdown.minutes == 0 && countdown.seconds == 0)
		{
			countdownState = ToggleStateDisabled;
			LedAllOn();
		}else
		{
			countdown = RtcCreateTime(hours_to_uint8_t(countdownFinish.hours) - hours_to_uint8_t(time.hours),
				minutes_to_uint8_t(countdownFinish.minutes) - minutes_to_uint8_t(time.minutes),
				seconds_to_uint8_t(countdownFinish.seconds) - seconds_to_uint8_t(time.seconds));
		}
	}
}

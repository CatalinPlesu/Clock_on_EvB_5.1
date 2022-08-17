#include <avr/interrupt.h>
#include <stddef.h>

#include "Rtc.h"
ISR(EXT_INT1_vect){

}

PCF8563ConfigHandle* pcf8563ConfigHandle = NULL;
void (*PtrAlarmFunction)(void) = NULL;
static Time time = {};

static Time timerStarted = { 0, 0 };
static Time timer = { 0, 0 };
static RtcTimerState rtcTimerState = RtcTimerStateIdle;

static Time alarm = { 4, 4 };

static Time countdown = { 3, 3 };

static uint8_t hours_to_uint8_t(uint8_t hours);
static uint8_t minutes_to_uint8_t(uint8_t minutes);
static uint8_t seconds_to_uint8_t(uint8_t seconds);

void RtcInit(void (*ptrAlarmFunction)(void)){
    pcf8563ConfigHandle = RtcCfgInitAndGet();
    alarm.minutes = pcf8563ConfigHandle->minute_alarm;
    alarm.hours = pcf8563ConfigHandle->hour_alarm;
    PtrAlarmFunction =  ptrAlarmFunction;

	GICR = (0x01<<INT1);					/* Enable INT1*/
	MCUCR = (1<<ISC11 | 0<<ISC10);	/* Trigger INT0 on rising edge */

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

void RtcReadTime(void)
{
    time = RtcCfgReadTime();
}

void RtcSetTime(Time desiredTime)
{
    RtcCfgWriteTime(desiredTime);
}

Time RtcCreateTime(int8_t hours, int8_t minutes, int8_t seconds)
{
    if (seconds < 0) {
        minutes -= 1;
        seconds += 60;
    }
    if (minutes < 0) {
        hours -= 1;
        minutes += 60;
    }
    if (hours < 0) {
        hours += 24;
    }

    Time time = {};
    time.hours = ((hours / 10) << 4) | (hours % 10);
    time.minutes = ((minutes / 10) << 4) | (minutes % 10);
    time.seconds = ((seconds / 10) << 4) | (seconds % 10);
    return time;
}

void RtcHoursTensIncrease(Time* time)
{
    time->hours = ((0x3 << 4) & (time->hours + (1 << 4))) | HOURS_MASK_UNITS(time->hours);
    if (HOURS_MASK_TENS(time->hours) >= 3)
        time->hours = HOURS_MASK_UNITS(time->hours);
    if (HOURS_MASK_TENS(time->hours) * 10 + HOURS_MASK_UNITS(time->hours) > 23)
        time->hours = 0;
}
void RtcHoursTensDecrease(Time* time)
{
    time->hours = ((0x3 << 4) & (time->hours - (1 << 4))) | HOURS_MASK_UNITS(time->hours);
    if (HOURS_MASK_TENS(time->hours) >= 3)
        time->hours = (2 << 4) | HOURS_MASK_UNITS(time->hours);
    if (HOURS_MASK_TENS(time->hours) * 10 + HOURS_MASK_UNITS(time->hours) > 23)
        time->hours = 0;
}

void RtcHoursUnitsIncrease(Time* time)
{
    time->hours = (0xf & (time->hours + 1)) | (HOURS_MASK_TENS(time->hours) << 4);
    if (HOURS_MASK_UNITS(time->hours) >= 10)
        time->hours = (HOURS_MASK_TENS(time->hours) << 4);
    if (HOURS_MASK_TENS(time->hours) * 10 + HOURS_MASK_UNITS(time->hours) > 23)
        time->hours = 0;
}

void RtcHoursUnitsDecrease(Time* time)
{
    time->hours = (0xf & (time->hours - 1)) | (HOURS_MASK_TENS(time->hours) << 4);
    if (HOURS_MASK_UNITS(time->hours) >= 10)
        time->hours = (HOURS_MASK_TENS(time->hours) << 4) | 9;
    if (HOURS_MASK_TENS(time->hours) * 10 + HOURS_MASK_UNITS(time->hours) > 23)
        time->hours = 0;
}

void RtcMinutesTensIncrease(Time* time)
{
    time->minutes = ((0x7 << 4) & (time->minutes + (1 << 4))) | MINUTES_MASK_UNITS(time->minutes);
    if (MINUTES_MASK_TENS(time->minutes) >= 6)
        time->minutes = MINUTES_MASK_UNITS(time->minutes);
}
void RtcMinutesTensDecrease(Time* time)
{
    time->minutes = ((0x7 << 4) & (time->minutes - (1 << 4))) | MINUTES_MASK_UNITS(time->minutes);
    if (MINUTES_MASK_TENS(time->minutes) >= 6)
        time->minutes = (5 << 4) | MINUTES_MASK_UNITS(time->minutes);
}

void RtcMinutesUnitsIncrease(Time* time)
{
    time->minutes = (0xf & (time->minutes + 1)) | (MINUTES_MASK_TENS(time->minutes) << 4);
    if (MINUTES_MASK_UNITS(time->minutes) >= 10)
        time->minutes = MINUTES_MASK_TENS(time->minutes);
}
void RtcMinutesUnitsDecrease(Time* time)
{
    time->minutes = (0xf & (time->minutes - 1)) | (MINUTES_MASK_TENS(time->minutes) << 4);
    if (MINUTES_MASK_UNITS(time->minutes) >= 10)
        time->minutes = (MINUTES_MASK_TENS(time->minutes) << 4) | 9;
}

static uint8_t hours_to_uint8_t(uint8_t hours)
{
    return (HOURS_MASK_TENS(hours) * 10 + HOURS_MASK_UNITS(hours));
}

static uint8_t minutes_to_uint8_t(uint8_t minutes)
{
    return (MINUTES_MASK_TENS(minutes) * 10 + MINUTES_MASK_UNITS(minutes));
}

static uint8_t seconds_to_uint8_t(uint8_t seconds)
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
    rtcTimerState = RtcTimerStateRunning;
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

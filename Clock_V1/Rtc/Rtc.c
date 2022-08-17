#include "Rtc.h"
#include "I2c.h"
/* #include "Twi.h" */
#include <avr/interrupt.h>
#include <stddef.h>

static Time time = {};
static Time timer = {5, 5};
static Time alarm = {4, 4};
static Time countdown = {3, 3};
	
static Time desiredTime = {};

void RtcInit(void)
{
    init_i2c();
	RtcReadTime();
}

void RtcReadTime(void)
{
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_MINUTES);
    i2c_start(RTC_SLA_R);
    time.minutes = i2c_readNack();
	i2c_start(RTC_SLA_R);
    time.hours = i2c_readNack();
    i2c_stop();
}

void RtcWriteTime(void){
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_SECUNDES);
	i2c_write(0);
    i2c_write(desiredTime.minutes);
    i2c_write(desiredTime.hours);
    i2c_stop();
}

Time* GetRtcTime(void)
{
    return &time;
}
Time* GetRtcTimer(void){
	return &timer;
}
Time* GetRtcAlarm(void){
	return &alarm;
}
Time* GetRtcCountdown(void){
	return &countdown;
}

/* void RtcSla(void) */
/* { */
/*     if (rtcTwiTask == RtcTwiTaskRead) { */
/*         TwiWrite(RTC_SLA_R); */
/*         rtcTwiData = RtcTwiDataMinutes; */
/*         rtcTwiTask = RtcTwiTaskWrite; */
/*     } else if (rtcTwiTask == RtcTwiTaskWrite) { */
/*         TwiWrite(RTC_SLA_W); */
/*         rtcTwiData = RtcTwiDataReg; */
/*     } */
/* } */

/* void RtcRegister(void) */
/* { */
/*     TwiWrite(MINUTES_REGISTER); */
/*     if (rtcTwiTaskData == RtcTwiTaskDataWrite) { */
/*         rtcTwiData = RtcTwiDataMinutes; */
/*     } else if (rtcTwiTaskData == RtcTwiTaskDataRead) { */
/*         rtcTwiTask = RtcTwiTaskRead; */
/*         rtcTwiState = RtcTwiStateRestart; */
/*     } */
/* } */

/* void RtcMinutes(void) */
/* { */
/*     if (rtcTwiTaskData == RtcTwiTaskDataRead) { */
/*         time.minutes.byte = TwiRead(); */
/*     } else if (rtcTwiTaskData == RtcTwiTaskDataWrite) { */
/*         TwiWrite(desiredTime.minutes.byte); */
/*     } */
/*     rtcTwiData = RtcTwiDataHours; */
/* } */

/* void RtcHours(void) */
/* { */
/*     if (rtcTwiTaskData == RtcTwiTaskDataRead) { */
/*         time.hours.byte = TwiRead(); */
/*     } else if (rtcTwiTaskData == RtcTwiTaskDataWrite) { */
/*         TwiWrite(desiredTime.hours.byte); */
/*         rtcTwiTaskData = RtcTwiTaskDataRead; */
/*         rtcTwiTask = RtcTwiTaskWrite; */
/*     } */
/*     rtcTwiState = RtcTwiStateStop; */
/* } */

void RtcSetTime(Time time)
{
    desiredTime = time;
	RtcWriteTime();
    /* rtcTwiTaskData = RtcTwiTaskDataWrite; */
}

Time RtcCreateTime(uint8_t hours, uint8_t minutes)
{
	Time time = {};
	time.hours = ((hours/10)<<4)|(hours%10);
	time.minutes = ((minutes/10)<<4)|(minutes%10);
	return time;
}

void RtcHoursTensIncrease(Time* time){
	time->hours=((0x3<<4)&(time->hours+(1<<4)))|HOURS_MASK_UNITS(time->hours);
	if(HOURS_MASK_TENS(time->hours)>=3)
		time->hours=HOURS_MASK_UNITS(time->hours);
	if(HOURS_MASK_TENS(time->hours)*10+HOURS_MASK_UNITS(time->hours)>23)
		time->hours = 0;
}
void RtcHoursTensDecrease(Time* time){
	time->hours=((0x3<<4)&(time->hours-(1<<4)))|HOURS_MASK_UNITS(time->hours);
	if(HOURS_MASK_TENS(time->hours)>=3)
		time->hours=(2<<4)|HOURS_MASK_UNITS(time->hours);
	if(HOURS_MASK_TENS(time->hours)*10+HOURS_MASK_UNITS(time->hours)>23)
		time->hours = 0;
}

void RtcHoursUnitsIncrease(Time* time){
	time->hours=(0xf&(time->hours+1))|(HOURS_MASK_TENS(time->hours)<<4);
	if(HOURS_MASK_UNITS(time->hours)>=10)
		time->hours=(HOURS_MASK_TENS(time->hours)<<4);
	if(HOURS_MASK_TENS(time->hours)*10+HOURS_MASK_UNITS(time->hours)>23)
		time->hours = 0;
}

void RtcHoursUnitsDecrease(Time* time){
	time->hours=(0xf&(time->hours-1))|(HOURS_MASK_TENS(time->hours)<<4);
	if(HOURS_MASK_UNITS(time->hours)>=10)
		time->hours=(HOURS_MASK_TENS(time->hours)<<4)|9;
	if(HOURS_MASK_TENS(time->hours)*10+HOURS_MASK_UNITS(time->hours)>23)
		time->hours = 0;
}

void RtcMinutesTensIncrease(Time* time){
	time->minutes=((0x7<<4)&(time->minutes+(1<<4)))|MINUTES_MASK_UNITS(time->minutes);
	if(MINUTES_MASK_TENS(time->minutes)>=6)
		time->minutes=MINUTES_MASK_UNITS(time->minutes);
}
void RtcMinutesTensDecrease(Time* time){
	time->minutes=((0x7<<4)&(time->minutes-(1<<4)))|MINUTES_MASK_UNITS(time->minutes);
	if(MINUTES_MASK_TENS(time->minutes)>=6)
		time->minutes=(5<<4)|MINUTES_MASK_UNITS(time->minutes);
}

void RtcMinutesUnitsIncrease(Time* time){
	time->minutes=(0xf&(time->minutes+1))|(MINUTES_MASK_TENS(time->minutes)<<4);
	if(MINUTES_MASK_UNITS(time->minutes)>=10)
		time->minutes=MINUTES_MASK_TENS(time->minutes);
}
void RtcMinutesUnitsDecrease(Time* time){
	time->minutes=(0xf&(time->minutes-1))|(MINUTES_MASK_TENS(time->minutes)<<4);
	if(MINUTES_MASK_UNITS(time->minutes)>=10)
		time->minutes=(MINUTES_MASK_TENS(time->minutes)<<4)|9;
}

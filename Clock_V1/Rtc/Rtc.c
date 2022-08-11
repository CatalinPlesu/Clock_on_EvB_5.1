#include "Rtc.h"
#include "I2c.h"
/* #include "Twi.h" */
#include <avr/interrupt.h>
#include <stddef.h>

static Time time = {};
static Time desiredTime = {};

/* RtcTwiState rtcTwiState = RtcTwiStateIdle; */
/* RtcTwiTask rtcTwiTask = RtcTwiTaskWrite; */
/* RtcTwiTaskData rtcTwiTaskData = RtcTwiTaskDataWrite; */
/* RtcTwiTaskData rtcTwiTaskData = RtcTwiTaskDataRead; */
/* RtcTwiData rtcTwiData = RtcTwiDataSla; */

/* void (*RtcFuncArray[])(void) = { */
/*     RtcSla, */
/*     RtcRegister, */
/*     RtcMinutes, */
/*     RtcHours */
/* }; */

/* ISR(TWI_vect) */
/* { */
/*     TwiStartClear(); */
/*     if (rtcTwiState == RtcTwiStateStop) { */
/*         TwiStop(); */
/*         rtcTwiState = RtcTwiStateRestart; */
/*     } else if (rtcTwiState == RtcTwiStateRestart) { */
/*         TwiStart(); */
/*         rtcTwiData = RtcTwiDataSla; */
/*         rtcTwiState = RtcTwiStateStart; */
/*     } else { */
/*         RtcFuncArray[rtcTwiData](); */
/*     } */

/*     // clears twint flag after all operations have been done */
/*     TwiClearInt(); */
/* } */

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

void TimeIncrement(Time* time)
{
    time->minutes++;
    TimeValidate(time);
}

void TimeDecrement(Time* time)
{
    time->minutes--;
    TimeValidate(time);
}

void TimeValidate(Time* time)
{
    if (time->minutes == 60) {
        time->minutes = 0;
        time->hours++;
    }
    if (time->minutes < 0) {
        time->minutes = 59;
        time->hours--;
    }
    if (time->hours >= 24)
        time->hours = 0;
    if (time->hours < 0)
        time->hours = 24;
}

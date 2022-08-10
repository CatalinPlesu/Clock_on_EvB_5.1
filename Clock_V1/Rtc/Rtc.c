#include "Rtc.h"
/* #include "Twi.h" */
#include <avr/interrupt.h>
#include <stddef.h>

static TimeBCD timeBcd = {};
static TimeBCD desiredTimeBcd = {};

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
    RtcWriteTime();
    RtcReadTime();
}

void RtcReadTime(void)
{
    i2c_start(RTC_SLA_W);
    i2c_write(MINUTES_REGISTER);
    i2c_start(RTC_SLA_R);
    timeBcd.minutes.byte = i2c_readNack();
    timeBcd.hours.byte = i2c_readNack();
    i2c_stop();
}

void RtcWriteTime(void){
    i2c_start(RTC_SLA_W);
    i2c_write(MINUTES_REGISTER);
    i2c_write(desiredTimeBcd.minutes.byte);
    i2c_write(desiredTimeBcd.hours.byte);
    i2c_stop();
}

TimeBCD* GetRtcTime(void)
{
    return &timeBcd;
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
/*         timeBcd.minutes.byte = TwiRead(); */
/*     } else if (rtcTwiTaskData == RtcTwiTaskDataWrite) { */
/*         TwiWrite(desiredTimeBcd.minutes.byte); */
/*     } */
/*     rtcTwiData = RtcTwiDataHours; */
/* } */

/* void RtcHours(void) */
/* { */
/*     if (rtcTwiTaskData == RtcTwiTaskDataRead) { */
/*         timeBcd.hours.byte = TwiRead(); */
/*     } else if (rtcTwiTaskData == RtcTwiTaskDataWrite) { */
/*         TwiWrite(desiredTimeBcd.hours.byte); */
/*         rtcTwiTaskData = RtcTwiTaskDataRead; */
/*         rtcTwiTask = RtcTwiTaskWrite; */
/*     } */
/*     rtcTwiState = RtcTwiStateStop; */
/* } */

void RtcSetTime(TimeBCD time)
{
    desiredTimeBcd = time;
    /* rtcTwiTaskData = RtcTwiTaskDataWrite; */
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

#include "RtcCfg.h"

static PCF8563ConfigHandle pcf8563ConfigHandle = {};

static void RtcCfgReadConfig(void)
{
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_CONTROL_STATUS_1);
    i2c_start(RTC_SLA_R);
    pcf8563ConfigHandle.control_status_1 = i2c_readNack();
    i2c_start(RTC_SLA_R);
    pcf8563ConfigHandle.control_status_2 = i2c_readNack();
    i2c_stop();
}

static void RtcCfgWriteConfig(void)
{
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_CONTROL_STATUS_1);
    i2c_write(pcf8563ConfigHandle.control_status_1);
    i2c_write(pcf8563ConfigHandle.control_status_2);
    i2c_stop();
}

PCF8563ConfigHandle* RtcCfgInitAndGet(void)
{
    init_i2c();
    RtcCfgReadConfig();
    RtcCfgReadTime();
    RtcCfgReadAlarm();

    return &pcf8563ConfigHandle;
}

Time RtcCfgReadTime(void)
{
    Time time = {};
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_SECUNDES);
    i2c_start(RTC_SLA_R);
    time.seconds = i2c_readNack();
    i2c_start(RTC_SLA_R);
    time.minutes = i2c_readNack();
    i2c_start(RTC_SLA_R);
    time.hours = i2c_readNack();
    i2c_stop();
    return time;
}

void RtcCfgWriteTime(Time time)
{
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_SECUNDES);
    i2c_write(time.seconds);
    i2c_write(time.minutes);
    i2c_write(time.hours);
    i2c_stop();
}

void RtcCfgReadAlarm(void)
{
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_ALARM_MINUTE);
    i2c_start(RTC_SLA_R);
    pcf8563ConfigHandle.minute_alarm = i2c_readNack();
    i2c_start(RTC_SLA_R);
    pcf8563ConfigHandle.hour_alarm = i2c_readNack();
    i2c_stop();
}

void RtcCfgWriteAlarm(Time alarm)
{
	pcf8563ConfigHandle.minute_alarm = alarm.minutes| 0x01<<7;
	pcf8563ConfigHandle.hour_alarm = alarm.hours| 0x01<<7;
    i2c_start(RTC_SLA_W);
    i2c_write(REGISTER_ALARM_MINUTE);
    i2c_write(pcf8563ConfigHandle.minute_alarm);
    i2c_write(pcf8563ConfigHandle.hour_alarm);
    i2c_stop();
}

void RtcCfgAlarmEnable(void)
{
    pcf8563ConfigHandle.control_status_2 |= (0x01 << AIE);
    RtcCfgWriteConfig();
}

void RtcCfgAlarmDisable(void)
{
    pcf8563ConfigHandle.control_status_2 &= ~(0x01 << AIE);
    RtcCfgWriteConfig();
}

void RtcCfgAlamFlagClear(void)
{
    pcf8563ConfigHandle.control_status_2 &= ~(0x01 << AF);
    RtcCfgWriteConfig();
}

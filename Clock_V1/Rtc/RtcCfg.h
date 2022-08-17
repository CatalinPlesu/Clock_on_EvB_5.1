#ifndef RTCCFG_H_
#define RTCCFG_H_

#include <stdint.h>
#include "I2c.h"

// PCF8563 32.768 kHz
#define RTC_SLA_R 0xA3
#define RTC_SLA_W 0xA2

#define AF 3
#define AIE 1

#define REGISTER_CONTROL_STATUS_1      0x00
#define REGISTER_CONTROL_STATUS_2      0x01
#define REGISTER_SECUNDES              0x02
#define REGISTER_MINUTES               0x03
#define REGISTER_HOURS                 0x04
#define REGISTER_ALARM_MINUTE          0x09
#define REGISTER_ALARM_HOUR            0x0a

#define SECONDS_MASK_TENS(byte) ((byte >> 4) & 0x7)
#define SECONDS_MASK_UNITS(byte) ((byte)&0xf)
#define MINUTES_MASK_TENS(byte) ((byte >> 4) & 0x7)
#define MINUTES_MASK_UNITS(byte) ((byte)&0xf)
#define HOURS_MASK_TENS(byte) ((byte >> 4) & 0x3)
#define HOURS_MASK_UNITS(byte) ((byte)&0xf)

typedef enum {
    RtcTimerStateIdle,
    RtcTimerStateRunning,
} RtcTimerState;

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
} Time;

typedef struct {
    uint8_t control_status_1;
    uint8_t control_status_2;
    uint8_t minute_alarm;
    uint8_t hour_alarm;
} PCF8563ConfigHandle;

PCF8563ConfigHandle* RtcCfgInitAndGet(void);
Time RtcCfgReadTime(void);
void RtcCfgWriteTime(Time time);

void RtcCfgReadAlarm(void);
void RtcCfgWriteAlarm(Time alarm);
void RtcCfgAlarmEnable(void);
void RtcCfgAlarmDisable(void);
void RtcCfgAlamFlagClear(void);

#endif

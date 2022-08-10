#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>
#include "RtcCfg.h"

#define SECONDS_MASK_TENS(byte) ((byte>>4)&0x7)
#define SECONDS_MASK_UNITS(byte) ((byte)&0xf)
#define MINUTES_MASK_TENS(byte) ((byte>>4)&0x7)
#define MINUTES_MASK_UNITS(byte) ((byte)&0xf)
#define HOURS_MASK_TENS(byte) ((byte>>4)&0x3)
#define HOURS_MASK_UNITS(byte) ((byte)&0xf)

typedef enum
{
	RtcTwiStateIdle,
	RtcTwiStateStart,
	RtcTwiStateRestart,
	RtcTwiStateStop,
	RtcTwiStateError,
}RtcTwiState;

typedef enum{
    RtcTwiTaskWrite,
    RtcTwiTaskRead,
}RtcTwiTask;

typedef enum{
    RtcTwiTaskDataWrite,
    RtcTwiTaskDataRead,
}RtcTwiTaskData;

typedef enum{
    RtcTwiDataSla,
    RtcTwiDataReg,
    RtcTwiDataMinutes,
    RtcTwiDataHours,
}RtcTwiData;

typedef struct{
    uint8_t hours;
    uint8_t minutes;
}Time;

void RtcInit(void);
void RtcReadTime(void);
void RtcWriteTime(void);
Time* GetRtcTime(void);

void RtcSla(void);
void RtcRegister(void);
void RtcMinutes(void);
void RtcHours(void);

void RtcSetTime(Time time);
Time RtcCreateTime(uint8_t hours, uint8_t minutes);

void TimeIncrement(Time* time);
void TimeDecrement(Time* time);
void TimeValidate(Time* time);

#endif

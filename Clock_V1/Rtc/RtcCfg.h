#ifndef RTCCFG_H_
#define RTCCFG_H_

//PCF8563 32.768 kHz 
#define RTC_SLA_R    0xA3
#define RTC_SLA_W    0xA2

#define REGISTER_SECUNDES  0x02
#define REGISTER_MINUTES   0x03
#define REGISTER_HOURS     0x04



#define SECONDS_MASK_TENS(byte) ((byte>>4)&0x7)
#define SECONDS_MASK_UNITS(byte) ((byte)&0xf)
#define MINUTES_MASK_TENS(byte) ((byte>>4)&0x7)
#define MINUTES_MASK_UNITS(byte) ((byte)&0xf)
#define HOURS_MASK_TENS(byte) ((byte>>4)&0x3)
#define HOURS_MASK_UNITS(byte) ((byte)&0xf)

#endif

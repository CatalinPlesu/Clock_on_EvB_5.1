#ifndef RTCCFG_H_
#define RTCCFG_H_

//PCF8563 32.768 kHz 
#define RTC_SLA_R    0xA3
#define RTC_SLA_W    0xA2

#define MINUTES_REGISTER 0x03

// CPU clock frequency in the slave must be at least 16 times higher than the
// SCL frequency 2kHz
// twbr 250 prescaler 2

#endif

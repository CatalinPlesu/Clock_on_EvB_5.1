#ifndef RTCTWI_H_
#define RTCTWI_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//PCF8563 32.768 kHz 
#define RTC_READ    0xA3
#define RTC_WRITE   0xA2

// CPU clock frequency in the slave must be at least 16 times higher than the
// SCL frequency 2kHz
// twbr 250 prescaler 2

void RtcTwiInit(void);

uint8_t RtcTwiRead(void);
void RtcTwiWrite(uint8_t byte);

#endif

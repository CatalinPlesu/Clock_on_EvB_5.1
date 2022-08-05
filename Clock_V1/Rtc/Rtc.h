#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

typedef struct{
    uint8_t hours;
    uint8_t minutes;
}Time;

void TimeIncrement(Time* time);
void TimeDecrement(Time* time);
void TimeValidate(Time* time);

#endif

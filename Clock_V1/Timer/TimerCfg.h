/*
 * TimerCfg.h
 *
 * Created: 7/6/2022 6:11:31 PM
 *  Author: alexei.guriev
 */ 


#ifndef TIMERCFG_H_
#define TIMERCFG_H_

#include <stdbool.h>
#include "TimerSw.h"

#define TTIMER_CFG_COUNTER_INIT_STATE 49535
void TimerInitCfg();
void TimerEnableCfg(bool state);
TimerSwInitParam * TimerGetIntervalPointerCfg(void);




#endif /* TIMERCFG_H_ */

/*******************************************************************************
 * includes
 ******************************************************************************/
#include "TimerSwBase.h"

/*******************************************************************************
 * functions
 ******************************************************************************/
TimerSwValue TimerSwBasePassed(
        const TimerSwValue timeCurr,
        const TimerSwValue timeStart)
{
    if (timeCurr >= timeStart)
    {
        return timeCurr - timeStart;
    }
    else
    {
        return (TIMER_SW_VALUE_MAX - timeStart) + 1 + timeCurr;
    }
}

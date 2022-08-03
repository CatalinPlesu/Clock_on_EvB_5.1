#ifndef TIMESWRBASE_H
#define TIMESWRBASE_H

/**
 * @defgroup TimerSwBase TimerSwBase
 *
 * @brief  Software timer base functionality.
 * @author MIC
 *
 * @{
 */

/*******************************************************************************
 * includes
 ******************************************************************************/
#include "TimerSw.h"

/*******************************************************************************
 * functions
 ******************************************************************************/

/**
 * @brief Computed the passed time of two values.
 *
 * @param[in] timeCurr  Current time.
 * @param[in] timeStart Start time.
 *
 * @return TimerSwValue
 */
TimerSwValue TimerSwBasePassed(
        const TimerSwValue timeCurr,
        const TimerSwValue timeStart);

/**
 * @}
 */

#endif /* TIMESWRBASE_H */

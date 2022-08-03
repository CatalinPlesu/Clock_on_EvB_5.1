
#ifndef TIMERSW_H_
#define TIMERSW_H_

#include "TimerSwCfg.h"
#include <stdbool.h>


/*******************************************************************************
 * includes
 ******************************************************************************/

/*******************************************************************************
 * defines
 ******************************************************************************/
/**
 * @def   TIMER_SW_INIT
 * @brief Initial value for a timer handle.
 */
#define TIMER_SW_INIT          ((TimerSwHandle){{0}})

/**
 * @def   TIMER_SW_VALUE_MAX
 * @brief Maximum possible timer value.
 */
#define TIMER_SW_VALUE_MAX     (TimerSwValue)(-1)

/*******************************************************************************
 * typedefs
 ******************************************************************************/

/*******************************************************************************
 * timer structures
 ******************************************************************************/

/**
 * @brief Initialization parameter.
 */
typedef struct {
    /**
     * @brief Interval of the timer.
     */
    TimerSwValue interval;

} TimerSwInitParam;

/**
 * @brief Timer handle.
 */
typedef struct {

    /**
     * @brief Indicated a proper initialization of the timer.
     */
    bool initialized;

    /**
     * @brief Indicates whether the timer is active or inactive.
     */
    bool active;

    /**
     * @brief The tick when the timer was started.
     */
    TimerSwValue start;

    /**
     * @brief The tick when the timer will expire.
     */
    TimerSwValue end;

    /**
     * @brief The timer interval.
     */
    TimerSwValue interval;
	
    /**
     * @brief The pointer to tick interval.
     */
    TimerSwValue const * pInterval;

} TimerSwHandle;

/*******************************************************************************
 * functions
 ******************************************************************************/

/**
 * @brief Get initialization dependencies init parameter.
 *
 * @param[in]  param  Initialization parameter.
 *
 * @return StatusError
 * @retval CplErrNone  Success.
 * @retval CplErrParam Invalid parameter supplied.
 */
StatusError TimerSwGetInitParam( TimerSwInitParam * const param);

/**
 * @brief Initialize a software timer.
 *
 * @param[in]  param  Initialization parameter.
 * @param[out] handle Resulting timer handle.
 *
 * @return StatusError
 * @retval CplErrNone  Success.
 * @retval CplErrParam Invalid parameter supplied.
 * @retval CplErrRange Invalid interval supplied, must be > 0.
 */
StatusError TimerSwInit(
        const TimerSwInitParam * const param,
              TimerSwHandle    * const handle);

/**
 * @brief Release the software timer.
 *
 * @param[in] handle Timer handle.
 *
 * @return StatusError
 * @retval CplErrNone   Success.
 * @retval CplErrParam  Invalid parameter.
 * @retval CplErrHandle Invalid handle supplied.
 */
StatusError TimerSwRelease(
        TimerSwHandle * const handle);

/**
 * @brief Start the timer.
 *
 * @param[in] handle    Timer handle.
 *
 * @return StatusError
 * @retval CplErrNone         Success.
 * @retval CplErrParam        Invalid parameter supplied.
 * @retval CplErrHandle       Handle error, timer not initialized.
 * @retval CplErrNotAvailable System ticker is not active or not available.
 * @retval CplErrIo           Hardware error.
 */
StatusError TimerSwStartup(
			  TimerSwHandle * const handle,
		const TimerSwValue iterval);

/**
 * @brief Stop the timer.
 *
 * @param[in] handle    Timer handle.
 *
 * @return StatusError
 * @retval CplErrNone   Everything is fine
 * @retval CplErrParam  A parameter is invalid
 * @retval CplErrHandle Handle error, timer not initialized.
 * @retval CplErrDenied Timer not active.
 */
StatusError TimerSwShutdown(TimerSwHandle * const handle);

/**
 * @brief Check if the timer is active.
 *
 * @param[in] handle       Timer handle.
 *
 * @return StatusError
 * @retval CplErrNone      Timer is active
 * @retval CplErrParam     Invalid parameter supplied.
 * @retval CplErrHandle    Invalid handle supplied.
 * @retval CplErrNotActive Timer is inactive.
 */
StatusError TimerSwIsActive(
        const TimerSwHandle * const handle);

/**
 * @brief Check if the timer is expired.
 *
 * @return StatusError
 * @retval CplErrNone   The timer is not expired.
 * @retval CplErrParam  Invalid parameter supplied.
 * @retval CplErrHandle Invalid handle supplied.
 * @retval CplErrDenied The timer is inactive.
 * @retval CplErrTime   The timer is expired.
 */
StatusError TimerSwIsExpired(
        const TimerSwHandle * const handle);

/**
 * @brief Determine the remaining time of the specified timer.
 *
 * @param[in]  handle          handle of desired timer.
 * @param[out] remainingTimeMs Remaining time of the timer in milliseconds.
 *
 * @return StatusError
 * @retval CplErrNone   The timer is not expired, remaining time supplied.
 * @retval CplErrParam  A parameter is invalid.
 * @retval CplErrHandle Invalid handle supplied.
 * @retval CplErrDenied The timer isn't active.
 * @retval CplErrTime   The timer is expired.
 */
StatusError TimerSwRemaining(
        const TimerSwHandle * const handle,
              TimerSwValue  * const remainingTimeMs);

/**
 * @brief Determine the time that passed by since the activation of the timer.
 *
 * @param[in]  handle     Handle of desired timer.
 * @param[out] passedTime Resulting passed timer.
 *
 * @return StatusError
 * @retval CplErrNone   The timer is active, passed time supplied.
 * @retval CplErrParam  A parameter is invalid
 * @retval CplErrDenied The timer isn't active.
 * @retval CplErrTime   The timer expired, passed time supplied.
 */
StatusError TimerSwPassed(
        const TimerSwHandle * const handle,
              TimerSwValue  * const passedTime);

/**
 * @}
 */

#endif /* TIMERSW_H_ */

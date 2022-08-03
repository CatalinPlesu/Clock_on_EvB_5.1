
/*******************************************************************************
 * includes
 ******************************************************************************/
#include "TimerSw.h"
#include <stdbool.h>
#include "TimerSwBase.h"

/*******************************************************************************
 * local vars
 ******************************************************************************/
// static CplSalSysTickStateIf * s_SysTickStateIf;


/*******************************************************************************
 * local functions
 ******************************************************************************/
static StatusError s_IsValidHandle(const TimerSwHandle * const handle)
{
    if (NULL == handle)
    {
        return StatusErrParam;
    }

    if (false == ((const TimerSwHandle *)handle)->initialized)
    {
        return StatusErrHandle;
    }

    return StatusErrNone;
}

static StatusError s_IsValidAndActiveHandle(const TimerSwHandle * const handle)
{
    const StatusError err = s_IsValidHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    if (false == ((const TimerSwHandle *)handle)->active)
    {
        return StatusErrDenied;
    }

    return StatusErrNone;
}

static StatusError s_GetTimerStateProp(
        const TimerSwHandle			* const handle,
              TimerSwValue          * const optArgOutPassedTimeMs,
              TimerSwValue          * const optArgOutRemainingTimeMs)
{
    // /*
    //  * s : start time
    //  * a : actual time
    //  * e : end time
    //  *
    //  * case 0 : |---X----------------| -> start == end, active == end, timeout
    //  * case 1 : |---s---------e--a---| -> start  < end, active >= end, timeout
    //  * case 2 : |-----a----s---e-----| -> start  < end, active >= end with overflow, timeout
    //  * case 3 : |----e-a---------s---| -> start  > end, active >= end and active < start, timeout
    //  */

    TimerSwValue ticks;
    TimerSwValue timeValRemainingMs = 0;
    StatusError         err                = StatusErrNone;

    ticks = *handle->pInterval;

    /* case 0 is covered by interval != 0 check*/

    /* case 1 & 2 */
    if (handle->start < handle->end)
    {
        /* case 1 */
        if (ticks >= handle->end)
        {
            err = StatusErrTime;
        }
        /* case 2 */
        else if (ticks < handle->start)
        {
            err = StatusErrTime;
        }
        /* no timeout */
        else
         {
             timeValRemainingMs = handle->end - ticks;
         }
     }
     /* case 3 */
     else
     {
         /* case 3 */
         if ((ticks >= handle->end) && (ticks < handle->start))
         {
             err = StatusErrTime;
         }
         /* no timeout */
         else if (ticks < handle->end)
         {
         	timeValRemainingMs = handle->end - ticks;
         }
         else
         {
         	timeValRemainingMs = (CPL_SAL_TIMER_VALUE_MAX - ticks) + handle->end + 1;
         }
     }

     if (NULL != optArgOutRemainingTimeMs)
     {
         *optArgOutRemainingTimeMs = timeValRemainingMs;
     }

     if (NULL != optArgOutPassedTimeMs)
     {
         *optArgOutPassedTimeMs = TimerSwBasePassed(ticks, handle->start);
     }

    return err;
}

/*******************************************************************************
 * local functions
 ******************************************************************************/
static StatusError s_Init(
        const TimerSwInitParam      * const param,
              TimerSwHandle			* const handleNA)
{
	handleNA->initialized = true;
	handleNA->pInterval = &param->interval;
	handleNA->interval = param->interval;
	
    return StatusErrNone;
}

static StatusError s_Startup(
			  TimerSwHandle		* const handleNA,
		const TimerSwValue interval)
{
    TimerSwValue tick = *handleNA->pInterval;
	

    handleNA->initialized = true;
    handleNA->active      = true;
    handleNA->start       = tick;
    handleNA->end         = tick + interval;
    handleNA->interval    = interval;
	
	return StatusErrNone;
};
    

static StatusError s_Shutdown(TimerSwHandle * const handleNA)
{
    if (false == handleNA->active)
    {
        return StatusErrDenied;
    }

    handleNA->active = false;
    return StatusErrNone;
}

static StatusError s_IsActive(const TimerSwHandle * const handleNA)
{
    return (handleNA->active) ? StatusErrNone : StatusErrNotActive;
}

/*******************************************************************************
 * functions
 ******************************************************************************/
StatusError TimerSwGetInitParam(TimerSwInitParam * const param)
{
	// if (NULL == param)
    // {
    //     return StatusErrParam;
    // }

    // param->sysTickIf = CplSalSysTickGetStateIf();

    // return StatusErrNone;
    return StatusErrNotImplemented;
}

StatusError TimerSwInit(
        const TimerSwInitParam * const param,
              TimerSwHandle    * const handle)
{
    if (NULL == param || NULL == handle)
    {
        return StatusErrParam;
    }

    return s_Init(param, handle);
}

StatusError TimerSwRelease(
        TimerSwHandle * const handle)
{
    const StatusError err = s_IsValidHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    *handle = TIMER_SW_INIT;
    return StatusErrNone;
}

StatusError TimerSwStartup(
			  TimerSwHandle * const handle,
		const TimerSwValue interval)
{
    const StatusError err = s_IsValidHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    return s_Startup(handle, interval);
}

StatusError TimerSwShutdown(TimerSwHandle * const handle)
{
    const StatusError err = s_IsValidHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    return s_Shutdown(handle);
}

StatusError TimerSwIsActive(
        const TimerSwHandle * const handle)
{
    const StatusError err = s_IsValidHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    return s_IsActive(handle);
}

StatusError TimerSwIsExpired(
        const TimerSwHandle * const handle)
{
    const StatusError err = s_IsValidAndActiveHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    return s_GetTimerStateProp(
            handle,
            NULL,
            NULL);
}

StatusError TimerSwRemaining(
        const TimerSwHandle * const handle,
              TimerSwValue  * const remainingTimeMs)
{
    StatusError err;

    if (NULL == remainingTimeMs)
    {
        return StatusErrParam;
    }

    err = s_IsValidAndActiveHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    return s_GetTimerStateProp(
            handle,
            NULL,
            remainingTimeMs);
}

StatusError TimerSwPassed(
        const TimerSwHandle * const handle,
              TimerSwValue  * const passedTimeMs)
{
    StatusError err;

    if (NULL == passedTimeMs)
    {
        return StatusErrParam;
    }

    err = s_IsValidAndActiveHandle(handle);

    if (StatusErrNone != err)
    {
        return err;
    }

    return s_GetTimerStateProp(
            handle,
            passedTimeMs,
            NULL);
}

/*
 * SevSeg.c
 *
 * Created: 7/13/2022 2:43:06 PM
 *  Author: alexei.guriev
 */ 

#include "SevSeg.h"
#include "Rtc.h"
#include "SevSegCfg.h"
#include "TimerCfg.h"
#include <math.h>

#define COMA_INDEX		10
#define MINUS_INDEX		11
#define ZERO_INDEX		0
#define MAX_DIGIT_VALUE 9


static uint8_t digitCount = 0;
static SevSegHandleConfig* disp7SegHandleConfig;
TimerSwHandle timerSwHandle;
uint8_t digitsValue[DISPLAY_7_SEGMENT_DIGITS_COUNT];
uint8_t segCode[12]= {
	//  . a  b  c  d  e  f  g  
	0b01111110,  // 0
	0b00110000,  // 1
	0b01101101,  // 2
	0b01111001,  // 3
	0b00110011,  // 4
	0b01011011,  // 5
	0b01011111,  // 6
	0b01110000,  // 7
	0b01111111,  // 8
	0b01111011,  // 9
	0b10000000,  // .
	0b00000001   // -
};

/* local functions */
static void TimeToBuff(Time time,uint8_t * data);

static void FloatToBuff(float value,uint8_t * data);

/* Global functions */
void SevSegInit(void)
{
	StatusError err;
	for(uint8_t index = 0; index < DISPLAY_7_SEGMENT_DIGITS_COUNT; index++)
	{
		digitsValue[index] = 0;
	}
	disp7SegHandleConfig = SevSegCfgInitAndGet();	
	
	TimerSwInitParam *pTimerSwInitParam = TimerGetIntervalPointerCfg();
	
	err = TimerSwInit(pTimerSwInitParam,&timerSwHandle);
	
	if (err == StatusErrNone)
	{
		TimerSwStartup(&timerSwHandle,DISPLAY_7_SEGMENT_TIMER_MS);
	}
	digitCount = 0;
}

void SevSegRutine(void)
{
	StatusError err;
	err = TimerSwIsExpired(&timerSwHandle);
	if (err == StatusErrTime)
	{
		if (digitCount >= DISPLAY_7_SEGMENT_DIGITS_COUNT)
		{
			digitCount = 0;
		}
		SevSegCfgAllDigitsOff();
		
		SevSegCfgSetDigitValue(digitsValue[digitCount]);
		
		SevSegCfgDigitOn(digitCount);
		
		TimerSwStartup(&timerSwHandle,DISPLAY_7_SEGMENT_TIMER_MS);
		
		digitCount++;
	}
}

StatusError SevSegSetTimeVal(Time time)
{
	TimeToBuff(time,digitsValue);
	
	return StatusErrNone;
}

StatusError SevSegSetTimeBcd(TimeBCD time){

    digitsValue[0] = segCode[time.hours.bits.tens];
    digitsValue[1] = segCode[time.hours.bits.units] | segCode[COMA_INDEX];
    digitsValue[2] = segCode[time.minutes.bits.tens];
    digitsValue[3] = segCode[time.minutes.bits.units];

	return StatusErrNone;
}

StatusError SevSegSetFloatVal(float value)
{
	FloatToBuff(value,digitsValue);
	
	return StatusErrNone;
}

StatusError SevSegSetByDigit(uint8_t digitIndex, uint8_t digitValue, bool withComa)
{
	uint8_t localDigitValue;
	if (digitIndex < DISPLAY_7_SEGMENT_DIGITS_COUNT)
	{
		if (digitValue > MAX_DIGIT_VALUE)
		{
			return StatusErrRange;
		}
		localDigitValue = segCode[digitValue];
		if (withComa)
		{
			localDigitValue |= segCode[COMA_INDEX];
		}
		
		digitsValue[digitIndex] = localDigitValue;
		return StatusErrNone;
	}
	else
	{
		return StatusErrIndex;
	}
}

StatusError SevSegSetByDigitCostum(uint8_t digitIndex, uint8_t digitValue)
{
	if (digitIndex < DISPLAY_7_SEGMENT_DIGITS_COUNT)
	{
		digitsValue[digitIndex] = digitValue;
		return StatusErrNone;
	}
	else
	{
		return StatusErrIndex;
	}
}

void TimeToBuff(Time time,uint8_t * data)
{
	uint8_t localdigitCount = 0;
	
    if(time.hours<10)
    {
        data[localdigitCount] = segCode[0];
        localdigitCount++;
    }else{
        uint8_t tmp = time.hours / 10;
        data[localdigitCount] = segCode[tmp];
        localdigitCount++;
        time.hours %= 10;
    }
    data[localdigitCount] = segCode[time.hours] | segCode[COMA_INDEX];
    localdigitCount++;
	
    if(time.minutes<10)
    {
        data[localdigitCount] = segCode[0];
        localdigitCount++;
    }else{
        uint8_t tmp = time.minutes / 10;
        data[localdigitCount] = segCode[tmp];
        localdigitCount++;
        time.minutes %= 10;
    }
    data[localdigitCount] = segCode[time.minutes];
    localdigitCount++;
	
	return;
}


static void FloatToBuff(float value,uint8_t * data)
{
	bool startCountIncrease = false;
	float decVal;
	uint8_t localdigitCount = 0;
	uint8_t tmp;
	float localVlaue, localdecValue, decimalMultipler;
	
	decimalMultipler = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1));
	
	if (value < 0)
	{
		value *= -1;
		data[localdigitCount] = segCode[MINUS_INDEX];
		localdigitCount++;
		decVal = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 2));
		
		if (value >= (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1)))
		{
			localVlaue = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1)) - 1;
		}
		else
		{
			localVlaue = value;
		}
	}
	else
	{
		decVal = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT - 1));
		
		if (value >= (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT)))
		{
			localVlaue = (float)pow((double)10,(double)(DISPLAY_7_SEGMENT_DIGITS_COUNT)) - 1;
		}
		else
		{
			localVlaue = value;
		}
	}
	
	localdecValue = (localVlaue * (float)decimalMultipler) - (float)((int)localVlaue * (int)decimalMultipler);
	
	while(decVal >= 1)
	{
		if (localVlaue >= decVal)
		{
			tmp = (uint8_t)(localVlaue / decVal);
			data[localdigitCount] = segCode[tmp];
			localdigitCount++;
			localVlaue -= (float)tmp * decVal;
			startCountIncrease = true;
		}
		else if(startCountIncrease)
		{
			data[localdigitCount] = segCode[ZERO_INDEX];
			localdigitCount++;
		}
		decVal /= 10;
		
	}
	
	if (localdigitCount >= DISPLAY_7_SEGMENT_DIGITS_COUNT)
	{
		return;
	}
	
	if (localdigitCount == 0)
	{
		data[localdigitCount] = 0;
		localdigitCount++;
	}
	
	data[localdigitCount - 1] |= segCode[COMA_INDEX];
	
	decVal = decimalMultipler / 10;
	
	while(localdigitCount <= DISPLAY_7_SEGMENT_DIGITS_COUNT)
	{
		tmp = (uint8_t)(localdecValue / decVal);
		data[localdigitCount] = segCode[tmp];
		localdigitCount++;
		localdecValue -= (float)tmp * decVal;
		decVal /= 10;
	}
	
	return;
}

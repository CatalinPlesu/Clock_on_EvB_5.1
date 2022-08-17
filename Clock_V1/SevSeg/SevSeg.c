/*
 * SevSeg.c
 *
 * Created: 7/13/2022 2:43:06 PM
 *  Author: alexei.guriev
 */ 

#include "SevSeg.h"
#include "SevSegCfg.h"
#include "TimerCfg.h"
#include "Rtc.h"
#include <math.h>

#define COMA_INDEX		10
#define MINUS_INDEX		11
#define CELSIUS_INDEX	12
#define ZERO_INDEX		0
#define MAX_DIGIT_VALUE 9


static uint8_t digitCount = 0;
static SevSegHandleConfig* sevsegHandleConfig;
TimerSwHandle timerSwHandle;
uint8_t digitsValue[SEVSEG_DIGITS_COUNT];
uint8_t segCode[13]= {
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
	0b00000001,   // -
	0b01001110,   // C
};

/* local functions */
static void FloatToBuff(float value,uint8_t * data);

/* Global functions */
void SevSegInit(void)
{
	StatusError err;
	for(uint8_t index = 0; index < sevsegHandleConfig->digitsCount; index++)
	{
		digitsValue[index] = 0;
	}
	sevsegHandleConfig = SevSegCfgInitAndGet();	
	
	TimerSwInitParam *pTimerSwInitParam = TimerGetIntervalPointerCfg();
	
	err = TimerSwInit(pTimerSwInitParam,&timerSwHandle);
	
	if (err == StatusErrNone)
	{
		TimerSwStartup(&timerSwHandle,SEVSEG_TIMER_MS);
	}
	digitCount = 0;
}

void SevSegRutine(void)
{
	StatusError err;
	err = TimerSwIsExpired(&timerSwHandle);
	if (err == StatusErrTime)
	{
		if (digitCount >= sevsegHandleConfig->digitsCount)
		{
			digitCount = 0;
		}
		SevSegCfgAllDigitsOff();
		
		SevSegCfgSetDigitValue(digitsValue[digitCount]);
		
		SevSegCfgDigitOn(digitCount);
		
		TimerSwStartup(&timerSwHandle,SEVSEG_TIMER_MS);
		
		digitCount++;
	}
}

StatusError SevSegSetTimeVal(uint8_t hours, uint8_t minutes){

	digitsValue[0] = segCode[HOURS_MASK_TENS(hours)];
	digitsValue[1] = segCode[HOURS_MASK_UNITS(hours)] | segCode[COMA_INDEX];
	digitsValue[2] = segCode[MINUTES_MASK_TENS(minutes)];
	digitsValue[3] = segCode[MINUTES_MASK_UNITS(minutes)];

	return StatusErrNone;
}

StatusError SevSegSetTemperatureVal(int8_t value){
	
	if(value<0){
		digitsValue[0] = segCode[MINUS_INDEX];
		value*=-1;
	}else {
		digitsValue[0] = 0x00;
	}
	if(value>=99){
			digitsValue[1] = segCode[9];
			digitsValue[2] = segCode[9];
	}
	else if (value <10) {
		digitsValue[1] = 0x00;
		digitsValue[2] = segCode[value];
	}
	else{
			digitsValue[1] = segCode[value/10];
			digitsValue[2] = segCode[value%10];
	}

	digitsValue[3] = segCode[CELSIUS_INDEX] | segCode[COMA_INDEX];

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
	if (digitIndex < sevsegHandleConfig->digitsCount)
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
	if (digitIndex < sevsegHandleConfig->digitsCount)
	{
		digitsValue[digitIndex] = digitValue;
		return StatusErrNone;
	}
	else
	{
		return StatusErrIndex;
	}
}

static void FloatToBuff(float value,uint8_t * data)
{
	bool startCountIncrease = false;
	float decVal;
	uint8_t localdigitCount = 0;
	uint8_t tmp;
	float localVlaue, localdecValue, decimalMultipler;
	
	decimalMultipler = (float)pow((double)10,(double)(sevsegHandleConfig->digitsCount - 1));
	
	if (value < 0)
	{
		value *= -1;
		data[localdigitCount] = segCode[MINUS_INDEX];
		localdigitCount++;
		decVal = (float)pow((double)10,(double)(sevsegHandleConfig->digitsCount - 2));
		
		if (value >= (float)pow((double)10,(double)(sevsegHandleConfig->digitsCount - 1)))
		{
			localVlaue = (float)pow((double)10,(double)(sevsegHandleConfig->digitsCount - 1)) - 1;
		}
		else
		{
			localVlaue = value;
		}
	}
	else
	{
		decVal = (float)pow((double)10,(double)(sevsegHandleConfig->digitsCount - 1));
		
		if (value >= (float)pow((double)10,(double)(sevsegHandleConfig->digitsCount)))
		{
			localVlaue = (float)pow((double)10,(double)(sevsegHandleConfig->digitsCount)) - 1;
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
	
	if (localdigitCount >= sevsegHandleConfig->digitsCount)
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
	
	while(localdigitCount <= sevsegHandleConfig->digitsCount)
	{
		tmp = (uint8_t)(localdecValue / decVal);
		data[localdigitCount] = segCode[tmp];
		localdigitCount++;
		localdecValue -= (float)tmp * decVal;
		decVal /= 10;
	}
	
	return;
}

/*
 * SevSegCfg.c
 *
 * Created: 7/13/2022 2:49:14 PM
 *  Author: alexei.guriev
 */ 
#include "SevSegCfg.h"

static SevSegHandleConfig sevsegHandleConfig;





SevSegHandleConfig* SevSegCfgInitAndGet()
{
	SevSegHandleConfig sevseg_handle_config = {
		.ddrSeg = { &SEVSEG_0_DDR, &SEVSEG_1_DDR, &SEVSEG_2_DDR, &SEVSEG_3_DDR, &SEVSEG_4_DDR, &SEVSEG_5_DDR,
			&SEVSEG_6_DDR, &SEVSEG_7_DDR },
		.portSeg = { &SEVSEG_0_PORT, &SEVSEG_1_PORT, &SEVSEG_2_PORT, &SEVSEG_3_PORT, &SEVSEG_4_PORT, &SEVSEG_5_PORT,
			&SEVSEG_6_PORT, &SEVSEG_7_PORT },
		.pinSegValue = { SEVSEG_0_PORT_VALUE, SEVSEG_1_PORT_VALUE, SEVSEG_2_PORT_VALUE, SEVSEG_3_PORT_VALUE,
			SEVSEG_4_PORT_VALUE, SEVSEG_5_PORT_VALUE, SEVSEG_6_PORT_VALUE, SEVSEG_7_PORT_VALUE, },
		.ddrDIgit={ &SEVSEG_DIGIT_1_DDR, &SEVSEG_DIGIT_2_DDR, &SEVSEG_DIGIT_3_DDR, &SEVSEG_DIGIT_4_DDR, },
		.portDigit={ &SEVSEG_DIGIT_1_PORT, &SEVSEG_DIGIT_2_PORT, &SEVSEG_DIGIT_3_PORT, &SEVSEG_DIGIT_4_PORT, },
		.pinDigitValue={ SEVSEG_DIGIT_1_PORT_VALUE, SEVSEG_DIGIT_2_PORT_VALUE, SEVSEG_DIGIT_3_PORT_VALUE,
			SEVSEG_DIGIT_4_PORT_VALUE, },
		.digitsCount = SEVSEG_DIGITS_COUNT
	};


	sevsegHandleConfig = sevseg_handle_config;
	
	// makes all segment ports output
	for(uint8_t i = 0; i < 8; i++){
		*sevsegHandleConfig.ddrSeg[i] |= (0x01 << sevsegHandleConfig.pinSegValue[i]);
	}
	
	// makes all digit ports output
	for(uint8_t i = 0; i < sevsegHandleConfig.digitsCount; i++){
		*sevsegHandleConfig.ddrDIgit[i] |=	(0x01 << sevsegHandleConfig.pinDigitValue[i]);
	}
	
	return &sevsegHandleConfig;
}

void SevSegCfgAllDigitsOff(void)
{
	for(uint8_t i = 0; i < sevsegHandleConfig.digitsCount; i++){
		*sevsegHandleConfig.portDigit[i] |= (0x01 << sevsegHandleConfig.pinDigitValue[i]);
	}
}

void SevSegCfgSetDigitValue(uint8_t value)
{
	for(uint8_t i = 0; i < 8; i++){
		#if (SEVSEG_SEGMENT_MODE == COMUN_POWER_SUPPLY)
			uint8_t bit_value = (~value)&(0x01 << sevsegHandleConfig.pinSegValue[i]);
		#else
			uint8_t bit_value = value&(0x01 << sevsegHandleConfig.pinSegValue[i]);
		#endif

		if(bit_value){
			*sevsegHandleConfig.portSeg[i] |= (0x01<<sevsegHandleConfig.pinSegValue[i]);
		}else
		{
			*sevsegHandleConfig.portSeg[i] &= ~(0x01<<sevsegHandleConfig.pinSegValue[i]);
		}
	}
}

void SevSegCfgDigitOn(uint8_t digit)
{
	if (digit < sevsegHandleConfig.digitsCount)
	{
		*sevsegHandleConfig.portDigit[digit] &= ~(0x01 << sevsegHandleConfig.pinDigitValue[digit]);		
	}	
}


#ifndef CONFIG_H_
#define CONFIG_H_

typedef enum
{	DeviceStateStartup,
	DeviceStateInit,
	DeviceStateSleep,
	DeviceStateWake,
}DeviceState;

typedef enum
{
	DeviceDisplayStateClock,
	DeviceDisplayStateTimer,
	DeviceDisplayStateAlarm,
	DeviceDisplayStateCountdown,
	DeviceDisplayStateTemperature,
}DeviceDisplayState;

typedef enum
{
	DisplayStateOff,
	DisplayStateNormal,
	DisplayStateEdit,
}DisplayState;

typedef enum
{
	EditStateHours,
	EditStateMinutes,
}EditState;

#endif /* CONFIG_H_ */

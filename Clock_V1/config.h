#ifndef CONFIG_H_
#define CONFIG_H_

typedef enum
{	DeviceStateStartup,
	DeviceStateInit,
	DeviceStateSleep,
	DeviceStateWake,
	DeviceStateDisplayClock,
	DeviceStateDisplayTimer,
	DeviceStateDisplayAlarm,
	DeviceStateDisplayContdown,
	DeviceStateDisplayTemperature,
}DeviceState;

typedef enum
{
	DisplayStateOff,
	DisplayStateNormal,
	DisplayStateEdit,
}DisplayState;

#endif /* CONFIG_H_ */

#include "Rtc.h"

void TimeIncrement(Time* time){
	time->minutes++;
	TimeValidate(time);
}

void TimeDecrement(Time* time){
	time->minutes--;
	TimeValidate(time);
}

void TimeValidate(Time* time){
	if(time->minutes==60){
		time->minutes=0;
		time->hours++;}
	if(time->minutes<0){
		time->minutes=59;
		time->hours--;
	}
	if (time->hours>=24)
		time->hours=0;
	if (time->hours<0)
		time->hours=24;
}
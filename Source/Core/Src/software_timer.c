/*
 * software_timer.c
 *
 *  Created on: 1 Oct 2023
 *      Author: HP
 */

#include "software_timer.h"

int CounterTimer[NUMBER_COUNTER];
short FlagTimer[NUMBER_COUNTER];

void SetTimerDefault(int duration, int index){
	CounterTimer[index] = duration;
	FlagTimer[index] = 0;
}
void TimerRunDefault(int index){
	if (CounterTimer[index] <= 0) return;
	CounterTimer[index]--;
	if (CounterTimer[index] <= 0){
		FlagTimer[index] = 1;
	}
}
short GetFlagTimerDefault(int index){
	return FlagTimer[index];
}


void SetTimer7SEG(int duration){
	SetTimerDefault(duration, TIMER_FOR_7SEG);
}
short GetFlagTimer7SEG(){
	return GetFlagTimerDefault(TIMER_FOR_7SEG);
}
void TimerRun7SEG(){
	TimerRunDefault(TIMER_FOR_7SEG);
}


void SetTimerLED(int duration){
	SetTimerDefault(duration, TIMER_FOR_LED);
}
short GetFlagTimerLED(){
	return GetFlagTimerDefault(TIMER_FOR_LED);
}
void TimerRunLED(){
	TimerRunDefault(TIMER_FOR_LED);
}


void TimerRun(){
	TimerRun7SEG();
	TimerRunLED();
}

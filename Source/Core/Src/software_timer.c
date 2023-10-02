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
	CounterTimer[index] = duration / TIMER_CYCLE;
	FlagTimer[index] = 0;
}
short GetFlagTimerDefault(int index){
	return FlagTimer[index];
}
void TimerRunDefault(int index){
	if (CounterTimer[index] <= 0) return;
	CounterTimer[index]--;
	if (CounterTimer[index] <= 0){
		FlagTimer[index] = 1;
	}
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


void SetTimerDOT(int duration){
	SetTimerDefault(duration, TIMER_FOR_DOT);
}
short GetFlagTimerDOT(){
	return GetFlagTimerDefault(TIMER_FOR_DOT);
}
void TimerRunDOT(){
	TimerRunDefault(TIMER_FOR_DOT);
}


void SetTimerLEDMX(int duration){
	SetTimerDefault(duration, TIMER_FOR_LEDMX);
}
short GetFlagTimerLEDMX(){
	return GetFlagTimerDefault(TIMER_FOR_LEDMX);
}
void TimerRunLEDMX(){
	TimerRunDefault(TIMER_FOR_LEDMX);
}


void SetTimerShift(int duration){
	SetTimerDefault(duration, TIMER_FOR_SHIFT);
}
short GetFlagTimerShift(){
	return GetFlagTimerDefault(TIMER_FOR_SHIFT);
}
void TimerRunShift(){
	TimerRunDefault(TIMER_FOR_SHIFT);
}



void TimerRun(){
	TimerRun7SEG();
	TimerRunLED();
	TimerRunDOT();
	TimerRunLEDMX();
	TimerRunShift();
}

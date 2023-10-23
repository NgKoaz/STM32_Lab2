/*
 * timer.c
 *
 *  Created on: 22 Oct 2023
 *      Author: HP
 */

#include "global.h"

short counter[NO_TIMER];
uint8_t flag[NO_TIMER];

uint8_t getFlagTimer(uint8_t index){
	return flag[index];
}
void setTimer(uint8_t index, short duration){
	flag[index] = 0;
	counter[index] = duration / TIMER_CYCLE;
}
void Abstract_TimerRun(uint8_t index){
	if (counter[index] <= 0) return;
	counter[index]--;
	if (counter[index] <= 0){
		flag[index] = 1;
	}
}
void timerRun(){
	Abstract_TimerRun(SCANNING_TIMER);
	Abstract_TimerRun(UPDATING_TIMER);
	Abstract_TimerRun(SHIFTING_TIMER);
}

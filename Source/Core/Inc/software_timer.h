/*
 * software_timer.h
 *
 *  Created on: 1 Oct 2023
 *      Author: HP
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define NUMBER_COUNTER 3
#define TIMER_FOR_7SEG 0
#define TIMER_FOR_LED 1
#define TIMER_FOR_DOT 2

extern short FlagTimer[NUMBER_COUNTER];

void SetTimer7SEG(int duration);
void SetTimerLED(int duration);
void SetTimerDOT(int duration);

short GetFlagTimer7SEG();
short GetFlagTimerLED();
short GetFlagTimerDOT();

void TimerRun();

#endif /* INC_SOFTWARE_TIMER_H_ */

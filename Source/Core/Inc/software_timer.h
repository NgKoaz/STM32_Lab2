/*
 * software_timer.h
 *
 *  Created on: 1 Oct 2023
 *      Author: HP
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_CYCLE 10
#define NUMBER_COUNTER 5
#define TIMER_FOR_7SEG 0
#define TIMER_FOR_LED 1
#define TIMER_FOR_DOT 2
#define TIMER_FOR_LEDMX 3
#define TIMER_FOR_SHIFT 4

extern short FlagTimer[NUMBER_COUNTER];

void SetTimer7SEG(int duration);
void SetTimerLED(int duration);
void SetTimerDOT(int duration);
void SetTimerLEDMX(int duration);
void SetTimerShift(int duration);

short GetFlagTimer7SEG();
short GetFlagTimerLED();
short GetFlagTimerDOT();
short GetFlagTimerLEDMX();
short GetFlagTimerShift();

void TimerRun();

#endif /* INC_SOFTWARE_TIMER_H_ */

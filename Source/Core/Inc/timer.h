/*
 * timer.h
 *
 *  Created on: 22 Oct 2023
 *      Author: HP
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "global.h"

#define NO_TIMER				3
#define	SCANNING_TIMER			0
#define UPDATING_TIMER			1
#define SHIFTING_TIMER			2

#define DUR_SCANNING			10
#define DUR_UPDATING			1000
#define DUR_SHIFTING			240

#define TIMER_CYCLE			10

uint8_t getFlagTimer(uint8_t index);
void setTimer(uint8_t index, short duration);
void timerRun();

#endif /* INC_TIMER_H_ */

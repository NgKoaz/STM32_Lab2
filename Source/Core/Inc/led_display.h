/*
 * led_display.h
 *
 *  Created on: Oct 22, 2023
 *      Author: HP
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "global.h"

#define TOTAL_LED	4

extern uint8_t buffer_7SEG[TOTAL_LED];

void led_Display_Init(void);
void scanning7SEG(void);

#endif /* INC_LED_DISPLAY_H_ */

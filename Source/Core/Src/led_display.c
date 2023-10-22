/*
 * led_display.c
 *
 *  Created on: Oct 22, 2023
 *      Author: HP
 */

#include "led_display.h"

#define _7SEG_GPIO 		GPIOB
#define EN_GPIO			GPIOA
#define TURN_OFF		SET
#define TURN_ON			RESET


const uint8_t pattern[10] = {
		1, 79, 18, 6, 76,
		36, 32, 15, 0, 4
};

const short _7SEG_Pin[7] = {
		A_7S_Pin, B_7S_Pin, C_7S_Pin, D_7S_Pin,
		E_7S_Pin, F_7S_Pin, G_7S_Pin
};
const short EN_Pin[TOTAL_LED] = {
		EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin,
		EN4_Pin, EN5_Pin
};

uint8_t buffer_7SEG[TOTAL_LED];
uint8_t led_index;

void led_Display_Init(void){
	for(int i = 0; i < TOTAL_LED; i++){
		buffer_7SEG[i] = 0;
		HAL_GPIO_WritePin(EN_GPIO, EN_Pin[i], TURN_OFF);
	}
	led_index = 0;
}

void updateDataBus7SEG(uint8_t index){
	uint8_t num = buffer_7SEG[index];
	uint8_t p_num = pattern[num];
	HAL_GPIO_WritePin(_7SEG_GPIO, G_7S_Pin, p_num & 0x1);
	p_num >>= 1;
	HAL_GPIO_WritePin(_7SEG_GPIO, F_7S_Pin, p_num & 0x1);
	p_num >>= 1;
	HAL_GPIO_WritePin(_7SEG_GPIO, E_7S_Pin, p_num & 0x1);
	p_num >>= 1;
	HAL_GPIO_WritePin(_7SEG_GPIO, D_7S_Pin, p_num & 0x1);
	p_num >>= 1;
	HAL_GPIO_WritePin(_7SEG_GPIO, C_7S_Pin, p_num & 0x1);
	p_num >>= 1;
	HAL_GPIO_WritePin(_7SEG_GPIO, B_7S_Pin, p_num & 0x1);
	p_num >>= 1;
	HAL_GPIO_WritePin(_7SEG_GPIO, A_7S_Pin, p_num & 0x1);
}

void scanning7SEG(void){
	//Turn off previous led
	HAL_GPIO_WritePin(EN_GPIO, EN_Pin[led_index], TURN_OFF);
	led_index++;
	if (led_index >= TOTAL_LED) {
		led_index = 0;
	}
	//update bus
	updateDataBus7SEG(led_index);
	//Turn on next led
	HAL_GPIO_WritePin(EN_GPIO, EN_Pin[led_index], TURN_ON);
}


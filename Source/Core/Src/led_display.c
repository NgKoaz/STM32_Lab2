/*
 * led_display.c
 *
 *  Created on: Oct 22, 2023
 *      Author: HP
 */

#include "led_display.h"


#define EN_GPIO			GPIOA
#define _7SEG_GPIO 		GPIOB
#define ENM_GPIO		GPIOA
#define ROW_GPIO		GPIOB
#define NUM_ROW			8
#define NUM_COL			8

#define TURN_OFF		SET
#define TURN_ON			RESET

/* FOR 7SEG SECTION */
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
uint8_t led_index = TOTAL_LED - 1;
/* FOR 7SEG SECTION */


/* FOR MATRIX LED */
const short ROW_Pin[NUM_ROW] = {
		ROW0_Pin, ROW1_Pin, ROW2_Pin, ROW3_Pin,
		ROW4_Pin, ROW5_Pin, ROW6_Pin, ROW7_Pin
};
const short ENM_Pin[NUM_COL] = {
		ENM0_Pin, ENM1_Pin, ENM2_Pin, ENM3_Pin,
		ENM4_Pin, ENM5_Pin, ENM6_Pin, ENM7_Pin
};


uint8_t pattern_MLED[NUM_COL * 4] = {
		//K
		0xFF, 0x00, 0x00, 0xE7,
		0xC3, 0x18, 0x3C, 0xFF,
		//H
		0xFF, 0x00, 0x00, 0xE7,
		0xE7, 0x00, 0x00, 0xFF,
		//0
		0xFF, 0xC3, 0x81, 0x3C,
		0x3C, 0x81, 0xC3, 0xFF,
		//A
		0xFF, 0xC0, 0x80, 0x33,
		0x33, 0x80, 0xC0, 0xFF
};
uint8_t col_index 		= NUM_COL - 1;
uint8_t pattern_index 	= 0;
/* FOR MATRIX LED */


/* COMMON */
void led_Display_Init(void){
	for(int i = 0; i < TOTAL_LED; i++){
		HAL_GPIO_WritePin(EN_GPIO, EN_Pin[i], TURN_OFF);
	}
	for (int i = 0; i < NUM_COL; i++){
		HAL_GPIO_WritePin(ENM_GPIO, ENM_Pin[i], TURN_OFF);
	}
}
/* COMMON */


/* FOR 7SEG SECTION 2 */
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
/* FOR 7SEG SECTION 2 */


/* FOR MATRIX LED 2*/
void shiftingLeftBuffer(void){
	pattern_index++;
	if (pattern_index >= NUM_COL * 4){
		pattern_index = 0;
	}
}
void updateRow(uint8_t index){
	uint8_t num = pattern_MLED[index];
	HAL_GPIO_WritePin(ROW_GPIO, ROW7_Pin, num & 0x1);
	num >>= 1;
	HAL_GPIO_WritePin(ROW_GPIO, ROW6_Pin, num & 0x1);
	num >>= 1;
	HAL_GPIO_WritePin(ROW_GPIO, ROW5_Pin, num & 0x1);
	num >>= 1;
	HAL_GPIO_WritePin(ROW_GPIO, ROW4_Pin, num & 0x1);
	num >>= 1;
	HAL_GPIO_WritePin(ROW_GPIO, ROW3_Pin, num & 0x1);
	num >>= 1;
	HAL_GPIO_WritePin(ROW_GPIO, ROW2_Pin, num & 0x1);
	num >>= 1;
	HAL_GPIO_WritePin(ROW_GPIO, ROW1_Pin, num & 0x1);
	num >>= 1;
	HAL_GPIO_WritePin(ROW_GPIO, ROW0_Pin, num & 0x1);
}
void scanningMLED(void){
	//Turn off column led
	HAL_GPIO_WritePin(ENM_GPIO, ENM_Pin[col_index], TURN_OFF);
	col_index++;
	if (col_index >= NUM_COL) {
		col_index = 0;
	}
	uint8_t tmp_idx = pattern_index + col_index;
	//update bus
	if (tmp_idx >= NUM_COL * 4){
		tmp_idx -= NUM_COL * 4;
	}
	updateRow(tmp_idx);
	//Turn on next column led
	HAL_GPIO_WritePin(EN_GPIO, ENM_Pin[col_index], TURN_ON);
}
/* FOR MATRIX LED 2*/



// 0: Turn on | 1: Turn off
/* K
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  0  0  1  1
 * 1  0  0  0  0  1  1  1
 * 1  0  0  0  0  1  1  1
 * 1  0  0  1  0  0  1  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 */
/* H
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  0  0  0  0  1
 * 1  0  0  0  0  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 */
/* O
 * 1  1  1  0  0  1  1  1
 * 1  1  0  0  0  0  1  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  1  0  0  0  0  0  1
 * 1  1  1  0  0  1  1  1
 */
/* A
 * 1  1  1  0  0  1  1  1
 * 1  1  0  0  0  0  1  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  0  0  0  0  1
 * 1  0  0  0  0  0  0  1
 * 1  0  0  1  1  0  0  1
 * 1  0  0  1  1  0  0  1
 */

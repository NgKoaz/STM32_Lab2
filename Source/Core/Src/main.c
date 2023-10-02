/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_7SEG 4
#define MAX_LED_MATRIX 8
#define DURATION_LED 1000
#define DURATION_7SEG 250
#define DURATION_DOT 1000
#define DURATION_LEDMX 10
#define DURATION_SHIFT 240
#define LENGTH_BUFFER_PATTERN (MAX_LED_MATRIX * 4)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
//--------SET GPIO & PIN--------
GPIO_TypeDef* GPIO_7SEG = GPIOB;
const short Pin_7SEG[7] = {
	_7SEG_A_Pin, _7SEG_B_Pin, _7SEG_C_Pin, _7SEG_D_Pin,
	_7SEG_E_Pin, _7SEG_F_Pin, _7SEG_G_Pin
};

GPIO_TypeDef* GPIO_EN = GPIOA;
const short Pin_EN[MAX_7SEG] = {
	EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin
};

GPIO_TypeDef* GPIO_ENM = GPIOA;
const short Pin_ENM[MAX_LED_MATRIX] = {
		ENM0_Pin, ENM1_Pin, ENM2_Pin, ENM3_Pin,
		ENM4_Pin, ENM5_Pin, ENM6_Pin, ENM7_Pin
};

GPIO_TypeDef* GPIO_ROW = GPIOB;
const short Pin_ROW[MAX_LED_MATRIX] = {
		ROW0_Pin, ROW1_Pin, ROW2_Pin, ROW3_Pin,
		ROW4_Pin, ROW5_Pin, ROW6_Pin, ROW7_Pin
};

//--------Buffer--------
// 0: Turn on | 1: Turn off
const short b[10] = {
		1, 79, 18, 6, 76,
		36, 32, 15, 0, 4
};
int _7SEG_Buffer[MAX_7SEG] = {
		1, 1, 1, 2
};
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
uint8_t LEDMX_Pattern[LENGTH_BUFFER_PATTERN] = {
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

uint8_t LEDMX_Buffer[MAX_LED_MATRIX] = {
		//K
		0xFF, 0x00, 0x00, 0xE7,
		0xC3, 0x18, 0x3C, 0xFF
};


//--------Another--------
int _7SEG_Index = 0;
int LEDMX_IndexCol = 0;
int shiftingIndex = 0;

//--------Clock--------
int hour = 0;
int minute = 0;
int second = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void SetUp(void);
void UpdateDataBus7SEG(int num);
void EnableOne7SEG(int LED_ID);
void DisplayOne7SEG(int status, int num);
void Update7SEG(int index);
void UpdateClockBuffer(void);
void ClockTickTick(void);

void EnableColumnLEDMatrix(int index_col);
void UpdateDataRowBus(uint8_t num);
void DisplayOneColumnLEDMatrix(int index, uint8_t num);
void UpdateLEDMatrix(int index);
void UpdateLEDMatrixBuffer(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  SetUp();
  while (1)
  {
	  if (GetFlagTimerLED()) {
		  //Invoked every one second.
		  SetTimerLED(DURATION_LED);
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  ClockTickTick();
	  }
	  if (GetFlagTimerDOT()){
		  //Invoked every one second.
		  SetTimerDOT(DURATION_DOT);
		  HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
	  }
	  if (GetFlagTimer7SEG()){
		  //Switching LED every 250ms
		  SetTimer7SEG(DURATION_7SEG);
		  _7SEG_Index = (_7SEG_Index + 1) % MAX_7SEG;
		  Update7SEG(_7SEG_Index);
	  }
	  if (GetFlagTimerLEDMX()){
		  //Switching column every 20ms
		  SetTimerLEDMX(DURATION_LEDMX);
		  LEDMX_IndexCol = (LEDMX_IndexCol + 1) % MAX_LED_MATRIX;
		  UpdateLEDMatrix(LEDMX_IndexCol);
	  }
	  if (GetFlagTimerShift()){
		  SetTimerShift(DURATION_SHIFT);
		  shiftingIndex = (shiftingIndex + 1) % LENGTH_BUFFER_PATTERN;
		  UpdateLEDMatrixBuffer();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|LED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, _7SEG_A_Pin|_7SEG_B_Pin|_7SEG_C_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|_7SEG_D_Pin|_7SEG_E_Pin|_7SEG_F_Pin
                          |_7SEG_G_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin DOT_Pin LED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|LED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : _7SEG_A_Pin _7SEG_B_Pin _7SEG_C_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin _7SEG_D_Pin _7SEG_E_Pin _7SEG_F_Pin
                           _7SEG_G_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = _7SEG_A_Pin|_7SEG_B_Pin|_7SEG_C_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|_7SEG_D_Pin|_7SEG_E_Pin|_7SEG_F_Pin
                          |_7SEG_G_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	TimerRun();
}
void SetUp(){
	SetTimerLED(DURATION_LED);
	SetTimer7SEG(DURATION_7SEG);
	SetTimerDOT(DURATION_DOT);
	SetTimerLEDMX(DURATION_LEDMX);
	SetTimerShift(DURATION_SHIFT);
	UpdateClockBuffer();
	UpdateLEDMatrixBuffer();
}
void UpdateDataBus7SEG(int num){
	if (num > 9 || num < 0) return;
	int sel = b[num];
	for (int i = 6; i >= 0; i--){
		HAL_GPIO_WritePin(GPIO_7SEG, Pin_7SEG[i], sel % 2);
		sel /= 2;
	}
}
void EnableOne7SEG(int LED_ID){
	for (int i = 0; i < MAX_7SEG; i++){
		if (i == LED_ID){
			HAL_GPIO_WritePin(GPIO_EN, Pin_EN[i], RESET);
		} else {
			HAL_GPIO_WritePin(GPIO_EN, Pin_EN[i], SET);
		}
	}
}
void DisplayOne7SEG(int index, int num){
	EnableOne7SEG(index);
	UpdateDataBus7SEG(num);
}
void Update7SEG(int index){
	if (index > 3) return;
	DisplayOne7SEG(index, _7SEG_Buffer[index]);
}
void UpdateClockBuffer(void){
	_7SEG_Buffer[0] = hour / 10;
	_7SEG_Buffer[1] = hour % 10;
	_7SEG_Buffer[2] = minute / 10;
	_7SEG_Buffer[3] = minute % 10;
}
void ClockTickTick(void){
	second++;
	if (second >= 60){
		second = 0;
		minute++;
	}
	if(minute >= 60){
		minute = 0;
		hour++;
	}
	if(hour >= 24){
		hour = 0;
	}
	UpdateClockBuffer();
}


void EnableColumnLEDMatrix(int index_col){
	for(int i = 0; i < MAX_LED_MATRIX; i++){
		if (i == index_col) {
			HAL_GPIO_WritePin(GPIO_ENM, Pin_ENM[i], RESET);
		} else {
			HAL_GPIO_WritePin(GPIO_ENM, Pin_ENM[i], SET);
		}
	}
}
void UpdateDataRowBus(uint8_t num){
	for (int i = MAX_LED_MATRIX - 1; i >= 0; i--){
		HAL_GPIO_WritePin(GPIO_ROW, Pin_ROW[i], num % 2);
		num /= 2;
	}
}
void DisplayOneColumnLEDMatrix(int index, uint8_t num){
	EnableColumnLEDMatrix(index);
	UpdateDataRowBus(num);
}
void UpdateLEDMatrix(int index){
	if (index > MAX_LED_MATRIX) return;
	DisplayOneColumnLEDMatrix(index, LEDMX_Buffer[index]);
}
void UpdateLEDMatrixBuffer(void){
	for (int i = 0; i < 8; i++){
		LEDMX_Buffer[i] = LEDMX_Pattern[(shiftingIndex + i) % LENGTH_BUFFER_PATTERN];
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

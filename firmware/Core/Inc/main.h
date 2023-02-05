/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FAKE_CD_Pin GPIO_PIN_2
#define FAKE_CD_GPIO_Port GPIOC
#define PATH_0_EN_Pin GPIO_PIN_0
#define PATH_0_EN_GPIO_Port GPIOA
#define PATH_1_EN_Pin GPIO_PIN_1
#define PATH_1_EN_GPIO_Port GPIOA
#define PATH_2_EN_Pin GPIO_PIN_2
#define PATH_2_EN_GPIO_Port GPIOA
#define PATH_3_EN_Pin GPIO_PIN_3
#define PATH_3_EN_GPIO_Port GPIOA
#define PATH_4_EN_Pin GPIO_PIN_4
#define PATH_4_EN_GPIO_Port GPIOA
#define ADC_CLK_Pin GPIO_PIN_5
#define ADC_CLK_GPIO_Port GPIOA
#define ADC_SDI_Pin GPIO_PIN_6
#define ADC_SDI_GPIO_Port GPIOA
#define ADC_SDO_Pin GPIO_PIN_7
#define ADC_SDO_GPIO_Port GPIOA
#define ADC_CS_Pin GPIO_PIN_4
#define ADC_CS_GPIO_Port GPIOC
#define SIGNAL_LED_1_Pin GPIO_PIN_5
#define SIGNAL_LED_1_GPIO_Port GPIOC
#define SIGNAL_LED_2_Pin GPIO_PIN_0
#define SIGNAL_LED_2_GPIO_Port GPIOB
#define SIGNAL_LED_3_Pin GPIO_PIN_1
#define SIGNAL_LED_3_GPIO_Port GPIOB
#define SIGNAL_LED_4_Pin GPIO_PIN_2
#define SIGNAL_LED_4_GPIO_Port GPIOB
#define SIGNAL_LED_5_Pin GPIO_PIN_10
#define SIGNAL_LED_5_GPIO_Port GPIOB
#define TEST_LOAD_1_Pin GPIO_PIN_12
#define TEST_LOAD_1_GPIO_Port GPIOB
#define TEST_LOAD_2_Pin GPIO_PIN_13
#define TEST_LOAD_2_GPIO_Port GPIOB
#define TEST_LOAD_3_Pin GPIO_PIN_14
#define TEST_LOAD_3_GPIO_Port GPIOB
#define TEST_LOAD_4_Pin GPIO_PIN_15
#define TEST_LOAD_4_GPIO_Port GPIOB
#define ADC_INT_Pin GPIO_PIN_6
#define ADC_INT_GPIO_Port GPIOC
#define ADC_CONVST_Pin GPIO_PIN_7
#define ADC_CONVST_GPIO_Port GPIOC
#define USER_BTN_1_Pin GPIO_PIN_8
#define USER_BTN_1_GPIO_Port GPIOA
#define USER_BTN_1_EXTI_IRQn EXTI9_5_IRQn
#define USER_BTN_2_Pin GPIO_PIN_9
#define USER_BTN_2_GPIO_Port GPIOA
#define USER_BTN_2_EXTI_IRQn EXTI9_5_IRQn
#define USER_BTN_3_Pin GPIO_PIN_10
#define USER_BTN_3_GPIO_Port GPIOA
#define USER_BTN_3_EXTI_IRQn EXTI15_10_IRQn
#define DETECT_SD_CARD_Pin GPIO_PIN_15
#define DETECT_SD_CARD_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

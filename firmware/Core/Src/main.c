/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sd_card.h"
#include "usbd_cdc_if.h"
#include "lcd_display.h"
#include "mcp3421.h"
#include "ads7280.h"
#include "current_meter.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SD_HandleTypeDef hsd;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SDIO_SD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char buffer[100];
int counter = 0;
FRESULT result;
uint8_t usbBuffer[] = "Hello USB!\n\r";
uint8_t debugBuffer[] = "Hello Debugger!\n\r";

int Debug_write(uint8_t * pointer, uint16_t length) {
	uint16_t i;

	for (i = 0; i < length; i++) {
		ITM_SendChar(*pointer++);
	}

	return i;
}

void SDCard_debug(char *log) {
//	HAL_UART_Transmit(&huart2, (uint8_t *)log, strlen(log), HAL_MAX_DELAY);
}
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
  MX_I2C1_Init();
  MX_SDIO_SD_Init();
  MX_USB_DEVICE_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();
	lcd_clear();

//  SDCard_capacity capacity = {0};
//
//  SDCard_mount("/");
//  SDCard_checkCapacity(&capacity);
//  result = SDCard_createFile("FILE1.TXT");
//
//  if (result != FR_OK) {
//	  Error_Handler();
//  }
//
//  result = SDCard_createFile("FILE2.TXT");
//
//  if (result != FR_OK) {
//	  Error_Handler();
//  }
//
//  SDCard_unmount("/");
//
//  Debug_write(debugBuffer, sizeof(debugBuffer));

  // Current Meter

  char Buffer[32];

  ADS7280_config adsCfg = { 0 };
  ADS7280_readConfig(&adsCfg);

  adsCfg.channelType = ADS7280_MANUAL_CHANNEL;
  adsCfg.triggerMode = ADS7280_MANUAL_TRIGGER;

  ADS7280_writeConfig(&adsCfg);
  ADS7280_readConfig(&adsCfg);
  ADS7280_selectInput1();

  // Voltage meter

  MCP3421_config config = {0};

  config.mode = MCP3421_MODE_CONTINUOUS;
  config.sampleRate = MCP3421_RATE_003_75;
  config.gain = MCP3421_GAIN_1X;

  MCP3421_writeConfig(&hi2c1, &config);
  HAL_Delay(100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  CurrentMeter_currentValue currentValue = { 0 };
  uint8_t activePath = 255;

  while (1)
  {
    // USBD_StatusTypeDef status = CDC_Transmit_FS(usbBuffer, sizeof(usbBuffer));

    // for (uint8_t i = 1; i<128; i++) {
    //   uint8_t ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 3, 5);

    //   if (ret == HAL_OK) {
    //     sprintf(Buffer, "0x%X", i);
    //     CDC_Transmit_FS(Buffer, sizeof(Buffer));
    //   }
    // }

    uint32_t measurement = MCP3421_readMeasurement(&hi2c1);
    sprintf(Buffer, "V: %ldmV       \n", measurement);
    CDC_Transmit_FS((uint8_t *) Buffer, sizeof(Buffer));
    lcd_setCursor(0,0);
		lcd_send_string(Buffer);

    CurrentMeter_read(&currentValue);
    // CurrentMeter_actualize();
    uint8_t newPath = CurrentMeter_getActivePath();
    sprintf(Buffer, "I: %d     ", currentValue.raw);
    CDC_Transmit_FS((uint8_t *) Buffer, sizeof(Buffer));
    lcd_setCursor(0,1);
		lcd_send_string(Buffer);

    if (newPath != activePath) {
      sprintf(Buffer, "%d", newPath);
      lcd_setCursor(15,1);
      lcd_send_string(Buffer);

      activePath = newPath;
    }

    HAL_Delay(1);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_4B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 12;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PATH_0_EN_Pin|PATH_1_EN_Pin|PATH_2_EN_Pin|PATH_3_EN_Pin
                          |PATH_4_EN_Pin|ADC_CLK_Pin|ADC_SDO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ADC_CS_Pin|ADC_CONVST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SIGNAL_LED_1_GPIO_Port, SIGNAL_LED_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SIGNAL_LED_2_Pin|SIGNAL_LED_3_Pin|SIGNAL_LED_4_Pin|SIGNAL_LED_5_Pin
                          |TEST_LOAD_1_Pin|TEST_LOAD_2_Pin|TEST_LOAD_3_Pin|TEST_LOAD_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : FAKE_CD_Pin ADC_INT_Pin */
  GPIO_InitStruct.Pin = FAKE_CD_Pin|ADC_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PATH_0_EN_Pin PATH_1_EN_Pin PATH_2_EN_Pin PATH_3_EN_Pin
                           PATH_4_EN_Pin ADC_CLK_Pin ADC_SDO_Pin */
  GPIO_InitStruct.Pin = PATH_0_EN_Pin|PATH_1_EN_Pin|PATH_2_EN_Pin|PATH_3_EN_Pin
                          |PATH_4_EN_Pin|ADC_CLK_Pin|ADC_SDO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ADC_SDI_Pin DETECT_SD_CARD_Pin */
  GPIO_InitStruct.Pin = ADC_SDI_Pin|DETECT_SD_CARD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ADC_CS_Pin SIGNAL_LED_1_Pin ADC_CONVST_Pin */
  GPIO_InitStruct.Pin = ADC_CS_Pin|SIGNAL_LED_1_Pin|ADC_CONVST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SIGNAL_LED_2_Pin SIGNAL_LED_3_Pin SIGNAL_LED_4_Pin SIGNAL_LED_5_Pin */
  GPIO_InitStruct.Pin = SIGNAL_LED_2_Pin|SIGNAL_LED_3_Pin|SIGNAL_LED_4_Pin|SIGNAL_LED_5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : TEST_LOAD_1_Pin TEST_LOAD_2_Pin TEST_LOAD_3_Pin TEST_LOAD_4_Pin */
  GPIO_InitStruct.Pin = TEST_LOAD_1_Pin|TEST_LOAD_2_Pin|TEST_LOAD_3_Pin|TEST_LOAD_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : USER_BTN_1_Pin USER_BTN_2_Pin USER_BTN_3_Pin */
  GPIO_InitStruct.Pin = USER_BTN_1_Pin|USER_BTN_2_Pin|USER_BTN_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

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

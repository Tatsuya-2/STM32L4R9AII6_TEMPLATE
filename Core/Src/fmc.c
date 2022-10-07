/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : FMC.c
 * Description        : This file provides code for the configuration
 *                      of the FMC peripheral.
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
#include "fmc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SRAM_HandleTypeDef hsram1;

/* FMC initialization function */
void MX_FMC_Init(void)
{
  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = { 0 };

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
   */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_PSRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_ENABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_DISABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_ENABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_ENABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.NBLSetupTime = 0;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.DataHoldTime = 0;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 2;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void)
{
  /* USER CODE BEGIN FMC_MspInit 0 */

  /* USER CODE END FMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  if (FMC_Initialized)
  {
    return;
  }
  FMC_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  PD0   ------> FMC_D2
  PD4   ------> FMC_NOE
  PD1   ------> FMC_D3
  PE4   ------> FMC_A20
  PE3   ------> FMC_A19
  PB7   ------> FMC_NL
  PD5   ------> FMC_NWE
  PD6   ------> FMC_NWAIT
  PD3   ------> FMC_CLK
  PF2   ------> FMC_A2
  PF1   ------> FMC_A1
  PF0   ------> FMC_A0
  PD7   ------> FMC_NE1
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PG4   ------> FMC_A14
  PG3   ------> FMC_A13
  PG5   ------> FMC_A15
  PG1   ------> FMC_A11
  PE10   ------> FMC_D7
  PD13   ------> FMC_A18
  PG2   ------> FMC_A12
  PD15   ------> FMC_D1
  PD14   ------> FMC_D0
  PG0   ------> FMC_A10
  PE9   ------> FMC_D6
  PE15   ------> FMC_D12
  PD12   ------> FMC_A17
  PD11   ------> FMC_A16
  PD10   ------> FMC_D15
  PF15   ------> FMC_A9
  PE8   ------> FMC_D5
  PE14   ------> FMC_D11
  PD9   ------> FMC_D14
  PD8   ------> FMC_D13
  PF14   ------> FMC_A8
  PE7   ------> FMC_D4
  PE13   ------> FMC_D10
  PF13   ------> FMC_A7
  PE12   ------> FMC_D9
  PF12   ------> FMC_A6
  PE11   ------> FMC_D8
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = PSRAM_NBL0_Pin | PSRAM_NBL1_Pin | PSRAM_A20_Pin | PSRAM_A19_Pin | D7_Pin | D6_Pin | D12_Pin |
                        D5_Pin | D11_Pin | D4_Pin | D10_Pin | D9_Pin | D8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = D2_Pin | PSRAM_OE_Pin | D3_Pin | PSRAM_WE_Pin | PSRAM_WAIT_Pin | PSRAM_CLK_Pin | PSRAM_NE1_Pin |
                        PSRAM_A18_Pin | D1_Pin | D0_Pin | PSRAM_A17_Pin | PSRAM_A16_Pin | D15_Pin | D14_Pin | D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = PSRAM_ADV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(PSRAM_ADV_GPIO_Port, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = PSRAM_A2_Pin | PSRAM_A1_Pin | PSRAM_A0_Pin | PSRAM_A3_Pin | PSRAM_A4_Pin | PSRAM_A5_Pin |
                        PSRAM_A9_Pin | PSRAM_A8_Pin | PSRAM_A7_Pin | PSRAM_A6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = PSRAM_A14_Pin | PSRAM_A13_Pin | PSRAM_A15_Pin | PSRAM_A11_Pin | PSRAM_A12_Pin | PSRAM_A10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* USER CODE BEGIN FMC_MspInit 1 */

  /* USER CODE END FMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* sramHandle)
{
  /* USER CODE BEGIN SRAM_MspInit 0 */

  /* USER CODE END SRAM_MspInit 0 */
  HAL_FMC_MspInit();
  /* USER CODE BEGIN SRAM_MspInit 1 */

  /* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void)
{
  /* USER CODE BEGIN FMC_MspDeInit 0 */

  /* USER CODE END FMC_MspDeInit 0 */
  if (FMC_DeInitialized)
  {
    return;
  }
  FMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();

  /** FMC GPIO Configuration
  PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
  PD0   ------> FMC_D2
  PD4   ------> FMC_NOE
  PD1   ------> FMC_D3
  PE4   ------> FMC_A20
  PE3   ------> FMC_A19
  PB7   ------> FMC_NL
  PD5   ------> FMC_NWE
  PD6   ------> FMC_NWAIT
  PD3   ------> FMC_CLK
  PF2   ------> FMC_A2
  PF1   ------> FMC_A1
  PF0   ------> FMC_A0
  PD7   ------> FMC_NE1
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PG4   ------> FMC_A14
  PG3   ------> FMC_A13
  PG5   ------> FMC_A15
  PG1   ------> FMC_A11
  PE10   ------> FMC_D7
  PD13   ------> FMC_A18
  PG2   ------> FMC_A12
  PD15   ------> FMC_D1
  PD14   ------> FMC_D0
  PG0   ------> FMC_A10
  PE9   ------> FMC_D6
  PE15   ------> FMC_D12
  PD12   ------> FMC_A17
  PD11   ------> FMC_A16
  PD10   ------> FMC_D15
  PF15   ------> FMC_A9
  PE8   ------> FMC_D5
  PE14   ------> FMC_D11
  PD9   ------> FMC_D14
  PD8   ------> FMC_D13
  PF14   ------> FMC_A8
  PE7   ------> FMC_D4
  PE13   ------> FMC_D10
  PF13   ------> FMC_A7
  PE12   ------> FMC_D9
  PF12   ------> FMC_A6
  PE11   ------> FMC_D8
  */

  HAL_GPIO_DeInit(GPIOE, PSRAM_NBL0_Pin | PSRAM_NBL1_Pin | PSRAM_A20_Pin | PSRAM_A19_Pin | D7_Pin | D6_Pin | D12_Pin |
                             D5_Pin | D11_Pin | D4_Pin | D10_Pin | D9_Pin | D8_Pin);

  HAL_GPIO_DeInit(GPIOD, D2_Pin | PSRAM_OE_Pin | D3_Pin | PSRAM_WE_Pin | PSRAM_WAIT_Pin | PSRAM_CLK_Pin |
                             PSRAM_NE1_Pin | PSRAM_A18_Pin | D1_Pin | D0_Pin | PSRAM_A17_Pin | PSRAM_A16_Pin | D15_Pin |
                             D14_Pin | D13_Pin);

  HAL_GPIO_DeInit(PSRAM_ADV_GPIO_Port, PSRAM_ADV_Pin);

  HAL_GPIO_DeInit(GPIOF, PSRAM_A2_Pin | PSRAM_A1_Pin | PSRAM_A0_Pin | PSRAM_A3_Pin | PSRAM_A4_Pin | PSRAM_A5_Pin |
                             PSRAM_A9_Pin | PSRAM_A8_Pin | PSRAM_A7_Pin | PSRAM_A6_Pin);

  HAL_GPIO_DeInit(GPIOG, PSRAM_A14_Pin | PSRAM_A13_Pin | PSRAM_A15_Pin | PSRAM_A11_Pin | PSRAM_A12_Pin | PSRAM_A10_Pin);

  /* USER CODE BEGIN FMC_MspDeInit 1 */

  /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* sramHandle)
{
  /* USER CODE BEGIN SRAM_MspDeInit 0 */

  /* USER CODE END SRAM_MspDeInit 0 */
  HAL_FMC_MspDeInit();
  /* USER CODE BEGIN SRAM_MspDeInit 1 */

  /* USER CODE END SRAM_MspDeInit 1 */
}
/**
 * @}
 */

/**
 * @}
 */

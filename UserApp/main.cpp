#include "common_inc.h"
#include <cstdio>

/* User Variables ---------------------------------------------------------*/

//! User-Timers

//! Debug LED LD3

//! Robot instance

/* Thread Definitions -----------------------------------------------------*/

/* Timer Callbacks -------------------------------------------------------*/

/* Interrupt Callbacks -------------------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13)
  {
    printf("USER-B1 Interrupt!\n");
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  }
}

/* Default Entry -------------------------------------------------------*/
void Main(void)
{
}

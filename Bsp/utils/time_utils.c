#include "time_utils.h"

__STATIC_INLINE uint32_t LL_SYSTICK_IsActiveCounterFlag(void)
{
  /*Determines if the COUNTFLAG bit is 1. If 1, the counter has been decremented to 0 at least once. The bit is
   * automatically cleared after it is read. */
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}

uint32_t micros(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  LL_SYSTICK_IsActiveCounterFlag();  // Clear the counter "overflow" flag bit.
  uint32_t m = HAL_GetTick();
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if (LL_SYSTICK_IsActiveCounterFlag())
  {
    m = HAL_GetTick();
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}

void delayMicroseconds(uint32_t us)
{
  us *= 23;
  while (us--)
    __NOP();
}

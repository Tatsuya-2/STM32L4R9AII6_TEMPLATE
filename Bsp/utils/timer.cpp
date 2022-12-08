#include "timer.hpp"

static TimerCallback_t timerCallbacks[4];

Timer::Timer(TIM_HandleTypeDef* _htim, uint32_t _freqHz)
{
  htim16.Instance = TIM16;
  htim17.Instance = TIM17;

  if (!((_htim->Instance == TIM16) || (_htim->Instance == TIM17)))
  {
    Error_Handler();
  }

  if (_freqHz < 1)
    _freqHz = 1;
  else if (_freqHz > 10000000)
    _freqHz = 10000000;

  htim = _htim;
  freq = _freqHz;

  CalcRegister(freq);

  HAL_TIM_Base_DeInit(_htim);
  _htim->Init.Prescaler = PSC - 1;
  _htim->Init.CounterMode = TIM_COUNTERMODE_UP;
  _htim->Init.Period = ARR - 1;
  _htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  _htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
}

void Timer::Start()
{
  htim->Instance->CNT = 0;
  HAL_TIM_Base_Start_IT(htim);
}

void Timer::Stop()
{
  HAL_TIM_Base_Stop_IT(htim);
  /// Check the parameters
  assert_param(IS_TIM_ALL_PERIPH(htim));
  /// Reset the Counter Register value
  htim->Instance->CNT = 0;
}

void Timer::CalcRegister(uint32_t _freq)
{
  float psc = 0.5;
  float arr;

  do
  {
    psc *= 2.0;
    arr = 60000000.0f / psc / (float)_freq;
  } while (arr > 65535);

  if (htim->Instance == TIM16 || htim->Instance == TIM17)  // APB2 @192MHz
  {
    PSC = (uint16_t)round((double)psc) * 2;
    ARR = (uint16_t)(60000000.0f / (float)_freq / psc);
  }
}

void Timer::SetCallback(TimerCallback_t _timerCallback)
{
  if (htim->Instance == TIM16)
  {
    timerCallbacks[0] = _timerCallback;
  }
  else if (htim->Instance == TIM17)
  {
    timerCallbacks[1] = _timerCallback;
  }
}

extern "C" void OnTimerCallback(TIM_TypeDef* timInstance)
{
  if (timInstance == TIM16)
  {
    timerCallbacks[0]();
  }
  else if (timInstance == TIM17)
  {
    timerCallbacks[1]();
  }
}

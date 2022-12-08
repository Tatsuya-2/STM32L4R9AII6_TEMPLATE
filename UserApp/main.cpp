#include "common_inc.h"
#include <cstdio>

/* User Variables ---------------------------------------------------------*/
BoardConfig_t boardConfig;

//! User-Timers
Timer timerHeartBeat(&htim16, 1);    // 1[Hz]
Timer timerControlLoop(&htim17, 1);  // 1[Hz]

//! Robot instance
Robot robot;

//! Robot update state semaphores
osSemaphoreId_t sem_update_control;

/* Thread Definitions -----------------------------------------------------*/
osThreadId_t controlMessageLoopTaskHandle;
void ThreadControlMessageLoop(void* argument)
{
  for (;;)
  {
    robot.commandHandler.ParseCommand(robot.commandHandler.Pop(osWaitForever));
  }
}

osThreadId_t controlLoopTaskHandle;
void ThreadControlLoop(void* argument)
{
  //! Init Rover
  robot.Init();

  for (;;)
  {
    osSemaphoreAcquire(sem_update_control, osWaitForever);
    robot.Update();
  }
}

/* Timer Callbacks -------------------------------------------------------*/
void timerHeartBeatCallback()
{
  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
void timerControlLoopCallback()
{
  osSemaphoreRelease(sem_update_control);
}

/* Interrupt Callbacks -------------------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13)
  {
    printf("USER-B1 Interrupt!\n");
    // HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
  }
}

/* Default Entry -------------------------------------------------------*/
void Main(void)
{
  // Init Semaphores
  sem_update_control = osSemaphoreNew(1U, 1U, NULL);

  // Init all communication staff, include USB-CDC/VCP/UART/CAN etc.
  InitCommunication();

  // Init & Run User Threads.
  const osThreadAttr_t controlLoopTask_attributes = {
    .name = "ControlLoopTask",
    .stack_size = 1000 * 4,
    .priority = (osPriority_t)osPriorityRealtime,  // robot control thread is critical, should be the highest
  };
  controlLoopTaskHandle = osThreadNew(ThreadControlLoop, nullptr, &controlLoopTask_attributes);

  const osThreadAttr_t controlMessageLoopTask_attributes = {
    .name = "ControlMessageLoopTask",
    .stack_size = 1000 * 4,
    .priority = (osPriority_t)osPriorityNormal,
  };
  controlMessageLoopTaskHandle = osThreadNew(ThreadControlMessageLoop, nullptr, &controlMessageLoopTask_attributes);

  // Start Timer Callbacks.
  timerHeartBeat.SetCallback(timerHeartBeatCallback);
  timerHeartBeat.Start();
  timerControlLoop.SetCallback(timerControlLoopCallback);
  timerControlLoop.Start();
}

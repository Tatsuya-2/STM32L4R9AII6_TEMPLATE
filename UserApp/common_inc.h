#ifndef STM32_COMMON_INC_H
#define STM32_COMMON_INC_H

#define CONFIG_FW_VERSION 1.0

/*---------------------------- C Scope ---------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "freertos_inc.h"
#include "tim.h"
#include "time_utils.h"

void Main(void);

#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/
#include "string"
#include "communication.hpp"
#include "timer.hpp"

// Universal Board Configs definition
struct BoardConfig_t
{
  uint8_t canNodeId = 1;
  bool userConfigLoaded = false;
};
extern struct BoardConfig_t boardConfig;

#endif
#endif  // STM32_COMMON_INC_H

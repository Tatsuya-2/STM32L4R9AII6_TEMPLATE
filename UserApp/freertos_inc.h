#ifndef __FREERTOS_H
#define __FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif

// List of semaphores
// List of Tasks
/*--------------------------------- System Tasks -------------------------------------*/
extern osThreadId_t defaultTaskHandle;  // Usage: 128 * 4 Bytes stack
/*---------------------------------- User Tasks --------------------------------------*/

/*---------------- 24.1K (used) / 64K (for FreeRTOS) / 128K (total) ------------------*/

#ifdef __cplusplus
}
#endif

#endif

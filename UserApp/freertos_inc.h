#ifndef __FREERTOS_H
#define __FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif

// List of semaphores
extern osSemaphoreId sem_usb_irq;
extern osSemaphoreId sem_uart2_dma;
extern osSemaphoreId sem_uart3_dma;
extern osSemaphoreId sem_usb_rx;
extern osSemaphoreId sem_usb_tx;

// List of Tasks
/*--------------------------------- System Tasks -------------------------------------*/
extern osThreadId_t defaultTaskHandle;     // Usage: 128 * 4 Bytes stack
extern osThreadId_t commTaskHandle;        // Usage: 3000 * 4 Bytes stack
extern osThreadId_t usbIrqTaskHandle;      // Usage: 128 * 4  Bytes stack
extern osThreadId_t usbServerTaskHandle;   // Usage: 512 * 4 Bytes stack
extern osThreadId_t uartServerTaskHandle;  // Usage: 1000 * 4 Bytes stack

/*---------------------------------- User Tasks --------------------------------------*/

/*---------------- 24.1K (used) / 64K (for FreeRTOS) / 128K (total) ------------------*/

#ifdef __cplusplus
}
#endif

#endif

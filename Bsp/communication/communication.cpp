
/* Includes ------------------------------------------------------------------*/

#include "communication.hpp"
#include "common_inc.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Global constant data ------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private constant data -----------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile bool endpointListValid = false;

/* Private function prototypes -----------------------------------------------*/
/* Function implementations --------------------------------------------------*/
// @brief Sends a line on the specified output.

osThreadId_t commTaskHandle;
const osThreadAttr_t commTask_attributes = {
  .name = "commTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t)osPriorityNormal,
};

void InitCommunication(void)
{
  // Start command handling thread
  commTaskHandle = osThreadNew(CommunicationTask, NULL, &commTask_attributes);

  while (!endpointListValid)
    osDelay(1);
}

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
osThreadId_t usbIrqTaskHandle;

void UsbDeferredInterruptTask(void* ctx)
{
  (void)ctx;  // unused parameter

  for (;;)
  {
    // Wait for signalling from USB interrupt (OTG_FS_IRQHandler)
    osStatus semaphore_status = osSemaphoreAcquire(sem_usb_irq, osWaitForever);
    if (semaphore_status == osOK)
    {
      // We have a new incoming USB transmission: handle it
      HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
      // Let the irq (OTG_FS_IRQHandler) fire again.
      HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
    }
  }
}

// Thread to handle deferred processing of USB interrupt, and
// read commands out of the UART DMA circular buffer
void CommunicationTask(void* ctx)
{
  (void)ctx;  // unused parameter

  CommitProtocol();

  // Allow main init to continue
  endpointListValid = true;

  StartUartServer();
  StartUsbServer();

  for (;;)
  {
    osDelay(1000);  // nothing to do
  }
}

extern "C" {
int _write(int file, const char* data, int len);
}

// @brief This is what printf calls internally
int _write(int file, const char* data, int len)
{
#if defined(DEBUG_VIA_USB_SERIAL)
  usbStreamOutputPtr->process_bytes((const uint8_t*)data, len, nullptr);
#elif defined(DEBUG_VIA_UART)
#if DEBUG_VIA_UART == 2
  uart2StreamOutputPtr->process_bytes((const uint8_t*)data, len, nullptr);
#endif
#endif
  return len;
}

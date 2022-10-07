#include "common_inc.h"
#include "interface_uart.hpp"
#include "ascii_processor.hpp"
#include "usart.h"
#include <vector>

static constexpr int UART_TX_BUFFER_SIZE = 64;
static constexpr int UART_RX_BUFFER_SIZE = 64;

// DMA open loop continuos circular buffer
// 1ms delay periodic, chase DMA ptr around
static uint8_t dma_rx_buffer[1][UART_RX_BUFFER_SIZE];
static uint32_t dma_last_rcv_idx[1];

// FIXME: the stdlib doesn't know about CMSIS threads, so this is just a global variable
// static thread_local uint32_t deadline_ms = 0;

osThreadId_t uartServerTaskHandle;

class UART2Sender : public StreamSink
{
public:
  UART2Sender()
  {
    channelType = CHANNEL_TYPE_UART2;
  }

  int process_bytes(const uint8_t* buffer, size_t length, size_t* processed_bytes) override
  {
    // Loop to ensure all bytes get sent
    while (length)
    {
      size_t chunk = length < UART_TX_BUFFER_SIZE ? length : UART_TX_BUFFER_SIZE;
      // wait for USB interface to become ready
      // TODO: implement ring buffer to get a more continuous stream of data
      // if (osSemaphoreWait(sem_uart_dma, deadline_to_timeout(deadline_ms)) != osOK)
      if (osSemaphoreAcquire(sem_uart2_dma, PROTOCOL_SERVER_TIMEOUT_MS) != osOK)
        return -1;
      // transmit chunk
      memcpy(tx_buf_, buffer, chunk);
      if (HAL_UART_Transmit_DMA(&huart2, tx_buf_, chunk) != HAL_OK)
        return -1;
      buffer += chunk;
      length -= chunk;
      if (processed_bytes)
        *processed_bytes += chunk;
    }
    return 0;
  }

  size_t get_free_space() override
  {
    return SIZE_MAX;
  }

private:
  uint8_t tx_buf_[UART_TX_BUFFER_SIZE];
} uart2_stream_output;

StreamSink* uart2StreamOutputPtr = &uart2_stream_output;
StreamBasedPacketSink uart2_packet_output(uart2_stream_output);
BidirectionalPacketBasedChannel uart2_channel(uart2_packet_output);
StreamToPacketSegmenter uart2_stream_input(uart2_channel);

static std::vector<UART_HandleTypeDef*> huart_list = { &huart2 };
static std::vector<StreamToPacketSegmenter> uart_stream_input_list = { uart2_stream_input };
static std::vector<StreamSink*> uart_stream_output_list = { &uart2_stream_output };

static void UartServerTask(void* ctx)
{
  (void)ctx;

  for (;;)
  {
    for (size_t i = 0; i < huart_list.size(); i++)
    {
      // Check for UART errors and restart receive DMA transfer if required
      if (huart_list[i]->ErrorCode != HAL_UART_ERROR_NONE)
      {
        HAL_UART_AbortReceive(huart_list[i]);
        HAL_UART_Receive_DMA(huart_list[i], dma_rx_buffer[i], sizeof(dma_rx_buffer[i]));
      }
      // Fetch the circular buffer "write pointer", where it would write next
      uint32_t new_rcv_idx = UART_RX_BUFFER_SIZE - huart_list[i]->hdmarx->Instance->CNDTR;

      // deadline_ms = timeout_to_deadline(PROTOCOL_SERVER_TIMEOUT_MS);
      // Process bytes in one or two chunks (two in case there was a wrap)
      if (new_rcv_idx < dma_last_rcv_idx[i])
      {
        uart_stream_input_list[i].process_bytes(dma_rx_buffer[i] + dma_last_rcv_idx[i],
                                                UART_RX_BUFFER_SIZE - dma_last_rcv_idx[i],
                                                nullptr);  // TODO: use process_all
        ASCII_protocol_parse_stream(dma_rx_buffer[i] + dma_last_rcv_idx[i], UART_RX_BUFFER_SIZE - dma_last_rcv_idx[i],
                                    *uart_stream_output_list[i]);
        dma_last_rcv_idx[i] = 0;
      }
      if (new_rcv_idx > dma_last_rcv_idx[i])
      {
        uart_stream_input_list[i].process_bytes(dma_rx_buffer[i] + dma_last_rcv_idx[i],
                                                new_rcv_idx - dma_last_rcv_idx[i],
                                                nullptr);  // TODO: use process_all
        ASCII_protocol_parse_stream(dma_rx_buffer[i] + dma_last_rcv_idx[i], new_rcv_idx - dma_last_rcv_idx[i],
                                    *uart_stream_output_list[i]);
        dma_last_rcv_idx[i] = new_rcv_idx;
      }
    }

    osDelay(1);
  };
}

const osThreadAttr_t uartServerTask_attributes = {
  .name = "UartServerTask",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t)osPriorityNormal,
};

void StartUartServer()
{
  // DMA is set up to receive in a circular buffer forever.
  // We dont use interrupts to fetch the data, instead we periodically read
  // data out of the circular buffer into a parse buffer, controlled by a state machine
  for (size_t i = 0; i < huart_list.size(); i++)
  {
    HAL_UART_Receive_DMA(huart_list[i], dma_rx_buffer[i], sizeof(dma_rx_buffer[i]));
    dma_last_rcv_idx[i] = UART_RX_BUFFER_SIZE - huart_list[i]->hdmarx->Instance->CNDTR;
  }

  // Start UART communication thread
  uartServerTaskHandle = osThreadNew(UartServerTask, nullptr, &uartServerTask_attributes);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
  if (huart->Instance == USART2)
    osSemaphoreRelease(sem_uart2_dma);
}

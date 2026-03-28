#include "Task_dma_usart.h"
#include "Task_common.h"
#include "./usart/bsp_usart.h"

void DMA_USART_Task(void *pvParameters) {
    uint8_t flag = 0;

    for (;;) {
        xQueueReceive(JPEG_DMA_QueueHandle, &flag, portMAX_DELAY);

        DMA_Cmd(DMA1_Channel4, DISABLE);
        DMA_ClearFlag(DMA1_FLAG_TC4);
        USART_ClearFlag(USART1, USART_FLAG_TC);
        DMA_SetCurrDataCounter(DMA1_Channel4, jpeg_chunk_len);
        DMA_Cmd(DMA1_Channel4, ENABLE);

        if (xSemaphoreTake(DMA_ISR_SemaphoreHandle, portMAX_DELAY) == pdTRUE) {
            // 传输完成
        }

        jpeg_chunk_len = 0;
    }
}
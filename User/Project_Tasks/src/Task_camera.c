#include "Task_camera.h"
#include "Task_common.h"
#include "bsp_ov7725.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include <string.h>



void ImageDispaly_Task(void *pvParameters) {
    for (;;) {
		if(in_camera_page == 1)
		{
			//printf("\r\n Image_Display gogogoog\r\n");
			if (Ov7725_vsync == 2) {
				xSemaphoreTake(LCD_MUTEX, portMAX_DELAY);
				xSemaphoreTake(DISPLAY_TRAN_MUTEX, portMAX_DELAY);
				FIFO_PREPARE;
                ILI9341_OpenWindow(0, 0, 320, 240);
                *(volatile uint16_t*)(FSMC_Addr_ILI9341_CMD) = 0x2C;
                for (int y = 0; y < 240; y++) {
                    for (int x = 0; x < 320; x++) {
                        uint16_t pixel;
                        READ_FIFO_PIXEL(pixel);
                        *(volatile uint16_t*)(FSMC_Addr_ILI9341_DATA) = pixel;
                    }
                }
				Ov7725_vsync = 0;
				xSemaphoreGive(DISPLAY_TRAN_MUTEX);
				xSemaphoreGive(LCD_MUTEX);
				uint8_t refresh_only_button = 1;
				xQueueSend(Refresh_Button_QueueHandle,&refresh_only_button,0);
			}
		}
        vTaskDelay(50);
    }
}

void FIFO_Read_Task(void *pvParameters) {
    static uint16_t RGB565 = 0;
    static uint8_t r5 = 0, g6 = 0, b5 = 0;
    static uint8_t count = 0;

    for (;;) {
        if (Ov7725_vsync == 2) {
            xSemaphoreTake(FIFO_DMA_SemaphoreHandle, portMAX_DELAY);

            if (count == 0) {
				xSemaphoreTake(Upload_SemaphoreHandle,portMAX_DELAY);
                xSemaphoreTake(DISPLAY_TRAN_MUTEX, portMAX_DELAY);
                FIFO_PREPARE;
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 320; j++) {
                        READ_FIFO_PIXEL(RGB565);
                        r5 = (RGB565 >> 11) & 0x1F;
                        g6 = (RGB565 >> 5) & 0x3F;
                        b5 = RGB565 & 0x1F;
                        camera_data[((i * 320) + j) * 3 + 0] = (r5 << 3) | (r5 >> 2);
                        camera_data[((i * 320) + j) * 3 + 1] = (g6 << 2) | (g6 >> 4);
                        camera_data[((i * 320) + j) * 3 + 2] = (b5 << 3) | (b5 >> 2);
                    }
                }
                count++;
            } else if (count < 30) {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 320; j++) {
                        READ_FIFO_PIXEL(RGB565);
                        r5 = (RGB565 >> 11) & 0x1F;
                        g6 = (RGB565 >> 5) & 0x3F;
                        b5 = RGB565 & 0x1F;
                        camera_data[((i * 320) + j) * 3 + 0] = (r5 << 3) | (r5 >> 2);
                        camera_data[((i * 320) + j) * 3 + 1] = (g6 << 2) | (g6 >> 4);
                        camera_data[((i * 320) + j) * 3 + 2] = (b5 << 3) | (b5 >> 2);
                    }
                }
                count++;
            } else {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 320; j++) {
                        READ_FIFO_PIXEL(RGB565);
                        r5 = (RGB565 >> 11) & 0x1F;
                        g6 = (RGB565 >> 5) & 0x3F;
                        b5 = RGB565 & 0x1F;
                        camera_data[((i * 320) + j) * 3 + 0] = (r5 << 3) | (r5 >> 2);
                        camera_data[((i * 320) + j) * 3 + 1] = (g6 << 2) | (g6 >> 4);
                        camera_data[((i * 320) + j) * 3 + 2] = (b5 << 3) | (b5 >> 2);
                    }
                }
                count = 0;
                Ov7725_vsync = 0;
            }

            xQueueSend(JPEG_FRAME_QueueHandle, &count, portMAX_DELAY);

            if (count == 0) {
                xSemaphoreGive(DISPLAY_TRAN_MUTEX);
                //vTaskDelay(10000);
            }
        }
        vTaskDelay(1);
    }
}
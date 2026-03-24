#include "Project_Tasks.h"
#define NDEBUG
#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"	
#include "bsp_ov7725.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "./usart/bsp_usart.h"
#include <string.h>
//外部变量声明 
extern volatile uint8_t Ov7725_vsync;
/*任务句柄*/
extern TaskHandle_t xAppTaskCreate_Handle;
extern TaskHandle_t ImageDispaly_Handle;
extern TaskHandle_t FIFO_Read_Handle;
extern TaskHandle_t JPEG_Handle;
extern TaskHandle_t DMA_USART_Handle;
extern TaskHandle_t ultimate_test_Handle;
/*消息队列句柄*/
extern QueueHandle_t JPEG_FRAME_QueueHandle;
extern QueueHandle_t JPEG_DMA_QueueHandle;
/*信号量句柄*/
extern SemaphoreHandle_t FIFO_DMA_SemaphoreHandle;
extern SemaphoreHandle_t DMA_ISR_SemaphoreHandle;

uint8_t camera_data[FIFO_INPUT_BUF_SIZE];
uint8_t jpeg_chunk_buffer[MAX_CHUNK_SIZE];
size_t  jpeg_chunk_len=0;
static void my_output_callback(void* context,void* data,int size)
{
	if(jpeg_chunk_len+size<=MAX_CHUNK_SIZE)
	{
		memcpy(jpeg_chunk_buffer+jpeg_chunk_len,data,size);
		jpeg_chunk_len+=size;
		
	}
	
}

void xAppTaskCreate_Task(void)
{
	BaseType_t xReturn=pdPASS;
	taskENTER_CRITICAL();
		/*消息队列创建*/
	JPEG_FRAME_QueueHandle=xQueueCreate(2,sizeof(uint8_t));
	JPEG_DMA_QueueHandle=xQueueCreate(2,sizeof(uint8_t));
	/*信号量创建*/
	FIFO_DMA_SemaphoreHandle=xSemaphoreCreateBinary();
	DMA_ISR_SemaphoreHandle=xSemaphoreCreateBinary();
//	xReturn=xTaskCreate(ImageDispaly_Task, "ImageDisplay", 128, NULL, 2, &ImageDispaly_Handle);
//	if(xReturn==pdPASS)
		//printf("\r\n终极测试任务创建成功\r\n");
	//JPEG_FRAME_QueueHandle=xQueueCreate(2,sizeof(uint8_t));
		xReturn=xTaskCreate((TaskFunction_t)FIFO_Read_Task,
						(const char *)"FIFO_Read_Task",
                        (uint32_t)256,
                        (void *)NULL,
                        (UBaseType_t)3,
                        (TaskHandle_t *)&FIFO_Read_Handle);
	if(xReturn==pdPASS)
	{
		//printf("\r\nFIFO_Read_Task created\r\n");
	}
	xReturn=pdPASS;
		xReturn=xTaskCreate((TaskFunction_t)JPEG_Task,
						(const char *)"JPEG_Task",
                        (uint32_t)2048,
                        (void *)NULL,
                        (UBaseType_t)4,
                        (TaskHandle_t *)&JPEG_Handle);
	if(xReturn==pdPASS)
	{
		//printf("\r\nJPEG_Task created\r\n");
	}
	else
	{
		//printf("\r\nJPEG_Task failed\r\n");
	}
	xReturn=pdPASS;
	xReturn=xTaskCreate((TaskFunction_t)DMA_USART_Task,
						(const char *)"DMA_USART_Task",
                        (uint32_t)256,
                        (void *)NULL,
                        (UBaseType_t)4,
                        (TaskHandle_t *)&DMA_USART_Handle);
	if(xReturn==pdPASS)
	{
		//printf("\r\ndMA_USART_Task created\r\n");
	}
	else
	{
		//printf("\r\nDMA_USART_Task failed\r\n");
	}
		xSemaphoreGive(FIFO_DMA_SemaphoreHandle);
		taskEXIT_CRITICAL();
			vTaskDelete(NULL);
}
//void ultimate_test(void *param)
//{
//	printf("终极测试任务启动！\r\n");
//	
//	while(1) {
//		printf("\r\nultimate_test\r\n");
//		//for(int i = 0; i < 1000000; i++);  // 忙等待
//		
//		vTaskDelay(1000);
//	}
//}
//void ImageDispaly_Task(void)
//{
//	for(;;)
//	{
//		/*接收到新图像进行显示*/
//		if( Ov7725_vsync == 2 )
//		{
//			FIFO_PREPARE;
//			ImagDisp(0,0,320,240);
//			Ov7725_vsync = 0;
//			
//		}
//		//vTaskDelay(1000);
//	}
//}
void FIFO_Read_Task(void)
{
	static uint16_t RGB565=0;
	static uint8_t r5=0;
	static uint8_t g6=0;
	static uint8_t b5=0;
	static uint8_t count=0;

	for(;;)
	{
		//printf("\r\nFIFO_Read_Task gogogo\r\n");

		//printf("\r\nFIFO_Read_Task gogogo\r\n");
		if( Ov7725_vsync == 2 )
		{
						 xSemaphoreTake(FIFO_DMA_SemaphoreHandle,
						portMAX_DELAY );
			//printf("\r\nFIFO_Read_Task gogogo\r\n");

			//printf("\r\nfirst\r\n");
			if(count==0)
			{
				vTaskDelay(1000);
				FIFO_PREPARE;  			/*FIFO准备*/
				for(int i=0;i<8;i++)
				{
					for(int j=0;j<320;j++)
					{
						READ_FIFO_PIXEL(RGB565);
						r5=(RGB565>>11)&(0x1F);
						g6=(RGB565>>5)&(0x3F);
						b5=RGB565&(0x1F);
						camera_data[((i*320)+j)*3+0]=(r5<<3)|(r5>>2);
						camera_data[((i*320)+j)*3+1]=(g6<<2)|(g6>>4);
						camera_data[((i*320)+j)*3+2]=(b5<<3)|(b5>>2);
					}
				}
				count++;
				//printf("\r\nFIFO_Read_Task first\r\n");
				xQueueSend(JPEG_FRAME_QueueHandle,
							(void *)&count,
							portMAX_DELAY);
							
				//xEventGroupSetBits(xFIFO_JPEG_EventGroup,FIFO_READ_COMPLETED_EVENT|FIFO_FIRST_COMPLETED);
			}
			else if(count<30)
			{
				for(int i=0;i<8;i++)
				{
					for(int j=0;j<320;j++)
					{
						READ_FIFO_PIXEL(RGB565);
						r5=(RGB565>>11)&(0x1F);
						g6=(RGB565>>5)&(0x3F);
						b5=RGB565&(0x1F);
						camera_data[((i*320)+j)*3+0]=(r5<<3)|(r5>>2);
						camera_data[((i*320)+j)*3+1]=(g6<<2)|(g6>>4);
						camera_data[((i*320)+j)*3+2]=(b5<<3)|(b5>>2);
					}
				}
				count++;
				//printf("\r\nFIFO_Read_Task second\r\n");
				xQueueSend(JPEG_FRAME_QueueHandle,
					   (void *)&count,
						portMAX_DELAY);
					   
				//xEventGroupSetBits(xFIFO_JPEG_EventGroup,FIFO_READ_COMPLETED_EVENT|FIFO_ROW_COMPLETED);
			}
			else
			{
				for(int i=0;i<8;i++)
				{
					for(int j=0;j<320;j++)
					{
						READ_FIFO_PIXEL(RGB565);
						r5=(RGB565>>11)&(0x1F);
						g6=(RGB565>>5)&(0x3F);
						b5=RGB565&(0x1F);
						camera_data[((i*320)+j)*3+0]=(r5<<3)|(r5>>2);
						camera_data[((i*320)+j)*3+1]=(g6<<2)|(g6>>4);
						camera_data[((i*320)+j)*3+2]=(b5<<3)|(b5>>2);
					}
				}
				count=0;
				//xEventGroupSetBits(xFIFO_JPEG_EventGroup,FIFO_READ_COMPLETED_EVENT|FIFO_FRAME_COMPLETED);
				Ov7725_vsync=0;
											//printf("\r\nFIFO_Read_Task third\r\n");
				xQueueSend(JPEG_FRAME_QueueHandle,
					   (void *)&count,
						portMAX_DELAY);
				//printf("\r\nFIFO_Read_Task second\r\n");
				vTaskDelay(30000);
			}

		}
		//else
			//printf("\r\nFIFO_Read_Task forth\r\n");
	}
}
void JPEG_Task(void)
{
	const int width = 320;
    const int height = 240;
    const int num_comp = 3;
    const int quality = 2;
	static uint8_t count=0;
	const uint8_t success=1;
	//static uint8_t event_bits;
	struct TJEEncoderState enc;
	for(;;)
	{
		//printf("\r\nJPEG_Task go gogo\r\n");
		//printf("\r\nJPEG_Task going\r\n");
		xQueueReceive(JPEG_FRAME_QueueHandle,
					  &count,
					  portMAX_DELAY);
		//printf("\r\ncount=%d\r\n",count);

		if(count==1)
		{
			//printf("\r\nsecond\r\n");
			while(!tje_encode_init(&enc, my_output_callback, NULL, quality,
                         width, height, num_comp, camera_data, FIFO_INPUT_BUF_SIZE));
			tje_encode_row_block(&enc);
					//printf("\r\nJPEG_Task first\r\n");
		}
		else if(count>1&&count<=30)
		{
			//printf("\r\nthird\r\n");
			tje_encode_row_block(&enc);
			//printf("\r\nJPEG_Task second\r\n");
		}
		else if(count==0)
		{
			tje_encode_row_block(&enc);
			tje_encode_finish(&enc);
			//FIFO_PREPARE;  			/*FIFO准备*/
			//printf("\r\nJPEG_Task third\r\n");
			
		}
		xQueueSend(JPEG_DMA_QueueHandle,
				   (void *)&success,
					portMAX_DELAY);
		//printf("\r\nJPEG_Task forth\r\n");
		//vTaskDelay(10000);

	}
}
void DMA_USART_Task(void )
{
	uint8_t flag=0;
	for(;;)
	{
							//printf("\r\nDMA_USART_Task going\r\n");
		xQueueReceive(JPEG_DMA_QueueHandle,
				   &flag,
					portMAX_DELAY);	
		//printf("\r\nDMA_USART_Task first\r\n");
		DMA_Cmd(DMA1_Channel4, DISABLE);
		// 3. 清除残留标志（避免重复触发中断）
		DMA_ClearFlag(DMA1_FLAG_TC4);
		USART_ClearFlag(USART1, USART_FLAG_TC);
		DMA_SetCurrDataCounter(DMA1_Channel4, jpeg_chunk_len);
		//printf("\r\nJPEG_LEN=%d\r\n",jpeg_chunk_len);
		//DMA_SetMemoryBaseAddr(DMA1_Channel4, (uint32_t)jpeg_chunk_buffer);
		DMA_Cmd(DMA1_Channel4, ENABLE);
				
		if(xSemaphoreTake(DMA_ISR_SemaphoreHandle, portMAX_DELAY) == pdTRUE) // 等待传输完成
		{
			//printf("\r\nDMA_USART_Task third\r\n");
		}
		//DMA_ClearFlag(DMA1_FLAG_TC4);             // 清除完成标志
		//printf("\r\nDMA_USART_Task forth\r\n");
		jpeg_chunk_len=0;
	}
}

//void ReadAndUpgrate(void)
//{
//	static uint16_t RGB565=0;
//	static uint8_t r5=0;
//	static uint8_t g6=0;
//	static uint8_t b5=0;
//	static uint8_t count=0;
//	const int width = 320;
//    const int height = 240;
//    const int num_comp = 3;
//    const int quality = 2;
//	//static uint8_t event_bits;
//	struct TJEEncoderState enc;
//	for(int i=0;i<8;i++)
//		{
//			for(int j=0;j<320;j++)
//			{
//				READ_FIFO_PIXEL(RGB565);
//				r5=(RGB565>>11)&(0x1F);
//				g6=(RGB565>>5)&(0x3F);
//				b5=RGB565&(0x1F);
//				camera_data[((i*320)+j)*3+0]=(r5<<3)|(r5>>2);
//				camera_data[((i*320)+j)*3+1]=(g6<<2)|(g6>>4);
//				camera_data[((i*320)+j)*3+2]=(b5<<3)|(b5>>2);
//			}
//		}
//		//printf("\r\nfirst\r\n");
//		if(count==0)
//		{
//			count++;
//			//xEventGroupSetBits(xFIFO_JPEG_EventGroup,FIFO_READ_COMPLETED_EVENT|FIFO_FIRST_COMPLETED);
//		}
//		else if(count<30)
//		{
//			count++;
//			//xEventGroupSetBits(xFIFO_JPEG_EventGroup,FIFO_READ_COMPLETED_EVENT|FIFO_ROW_COMPLETED);
//		}
//		else
//		{
//			count=0;
//			//xEventGroupSetBits(xFIFO_JPEG_EventGroup,FIFO_READ_COMPLETED_EVENT|FIFO_FRAME_COMPLETED);
//			Ov7725_vsync=0;
//		}
//		if(count==1)
//		{
//			//printf("\r\nsecond\r\n");
//			while(!tje_encode_init(&enc, my_output_callback, NULL, quality,
//                         width, height, num_comp, camera_data, FIFO_INPUT_BUF_SIZE));
//			tje_encode_row_block(&enc);
//		}
//		else if(count>1&&count<=30)
//		{
//			//printf("\r\nthird\r\n");
//			tje_encode_row_block(&enc);
//		}
//		else if(count==0)
//		{
//			tje_encode_row_block(&enc);
//			tje_encode_finish(&enc);
//			FIFO_PREPARE;  			/*FIFO准备*/
//		}
//		//printf("\r\nforth\r\n");
//		DMA_Cmd(DMA1_Channel4, DISABLE);
//		    // 3. 清除残留标志（避免重复触发中断）
//		DMA_ClearFlag(DMA1_FLAG_TC4);
//		USART_ClearFlag(USART1, USART_FLAG_TC);
//		DMA_SetCurrDataCounter(DMA1_Channel4, jpeg_chunk_len);
//		//DMA_SetMemoryBaseAddr(DMA1_Channel2, (uint32_t)jpeg_chunk_buffer);
//		DMA_Cmd(DMA1_Channel4, ENABLE);
//    while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET); // 等待传输完成
//    DMA_ClearFlag(DMA1_FLAG_TC4);             // 清除完成标志
//		jpeg_chunk_len=0;
//		
//}

/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   摄像头火眼ov7725测试例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  #include <stdint.h>
#include "stm32f10x.h"
//FreeRTOS头文件	
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "./ov7725/bsp_ov7725.h"
#include "bsp_dma.h"
#include "bsp_esp8266.h"
#include "Common.h"
#include "bsp_tim2.h"

#include "./lcd/bsp_ili9341_lcd.h"
#include "./lcd/bsp_xpt2046_lcd.h"
#include "./led/bsp_led.h"   
#include "./usart/bsp_usart.h"
#include "./key/bsp_key.h"  
#include "Task_appcreate.h"
#include "Task_common.h"
//#include "./systick/bsp_SysTick.h"
#include "./dwt_delay/core_delay.h"
#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
#include "lv_port_indev.h"       // LVGL的触屏支持
#include "gui_guider.h"           // Gui Guider 生成的界面和控件的声明
#include "events_init.h"          // Gui Guider 生成的初始化事件、回调函数
lv_ui  guider_ui;                     // 声明 界面对象


extern volatile uint8_t Ov7725_vsync;

//unsigned int Task_Delay[NumOfTask]; 

extern OV7725_MODE_PARAM cam_mode;
//void test_startup_task(void *param);

static void bsp_init (void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
//void ultimate_test(void *param);
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  * 提示  ：不要乱盖PC0跳帽！！
  */
int main(void) 	
{		
	
	//float frame_count = 0;
	BaseType_t xReturn=pdPASS;
	bsp_init();
	xReturn=xTaskCreate((TaskFunction_t)xAppTaskCreate_Task,
						(const char *)"AppTaskCreate_Task",
                        (uint32_t)64,
                        (void *)NULL,
                        (UBaseType_t)4,
                        (TaskHandle_t *)&xAppTaskCreate_Handle);

	Ov7725_vsync = 0;
	FIFO_PREPARE;  			/*FIFO准备*/
	if(xReturn==pdPASS)
		vTaskStartScheduler();
	while(1)
	{

		
	}
}
static void bsp_init ( void)
{
	uint8_t retry = 0;
	/* 液晶初始化 */
	ILI9341_Init();
	ILI9341_GramScan ( 3 );
	ILI9341_Clear(0,0,LCD_X_LENGTH, LCD_Y_LENGTH);
	XPT2046_Init();
	TIM2_Init_xms(1);
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	lv_init();                             // LVGL 初始化
	lv_port_disp_init();                   // 注册LVGL的显示任务
	lv_port_indev_init();                  // 注册LVGL的触屏检测任务
	setup_ui(&guider_ui);           // 初始化 UI
	events_init(&guider_ui);       // 初始化 事件
	
	/********显示字符串示例*******/
	CPU_TS_TmrInit();
	USART_Config();
	LED_GPIO_Config();
	Key_GPIO_Config();
	
	
	/* ov7725 gpio 初始化 */
	OV7725_GPIO_Config();
	USART3_DMA_Config();
	LED_BLUE;
	/* ov7725 寄存器默认配置初始化 */
	while(OV7725_Init() != SUCCESS)
	{
		retry++;
		if(retry>5)
		{
			printf("\r\n没有检测到OV7725摄像头\r\n");
//			ILI9341_DispStringLine_EN(LINE(2),"No OV7725 module detected!");
			while(1);
		}
	}


	/*根据摄像头参数组配置模式*/
	OV7725_Special_Effect(cam_mode.effect);
	/*光照模式*/
	OV7725_Light_Mode(cam_mode.light_mode);
	/*饱和度*/
	OV7725_Color_Saturation(cam_mode.saturation);
	/*光照度*/
	OV7725_Brightness(cam_mode.brightness);
	/*对比度*/
	OV7725_Contrast(cam_mode.contrast);
	/*特殊效果*/
	OV7725_Special_Effect(cam_mode.effect);
	
	/*设置图像采样及模式大小*/
	OV7725_Window_Set(cam_mode.cam_sx,
														cam_mode.cam_sy,
														cam_mode.cam_width,
														cam_mode.cam_height,
														cam_mode.QVGA_VGA);

//	/* 设置液晶扫描模式 */
//	ILI9341_GramScan( cam_mode.lcd_scan );
//	
//	
//	
//	ILI9341_DispStringLine_EN(LINE(2),"OV7725 initialize success!");
	printf("\r\nOV7725摄像头初始化完成\r\n");

}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("Stack Overflow: %s\r\n", pcTaskName);
    taskDISABLE_INTERRUPTS();
    for(;;);
}

/*********************************************END OF FILE**********************/


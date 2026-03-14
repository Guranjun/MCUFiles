/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
//FreeRTOS头文件	
#include "FreeRTOS.h"
#include "Task.h"

//BSP头文件
#include "bsp_led.h"
#include "bsp_usart.h"

/*         任务句柄        */
//创建任务句柄
static TaskHandle_t AppTaskCreate_Task_Handle;
//LED任务句柄
static TaskHandle_t LED1_Task_Handle;


/*内核对象句柄*/

/*全局变量声明*/


/*函数声明*/
static void BSP_init(void);
static void AppTaskCreate_Task(void * parameter);
static void LED1_Task(void * parameter);

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	BaseType_t xReturn=pdPASS;
	BSP_init();//BSP硬件初始化
  
  xReturn=xTaskCreate((TaskFunction_t)AppTaskCreate_Task,
										                          (const char *)"AppTaskCreate_Task",
                                              (uint32_t)512,
                                              (void *)NULL,
                                              (UBaseType_t)3,
                                              (TaskHandle_t *)AppTaskCreate_Task_Handle);
  if(xReturn==pdPASS)
    vTaskStartScheduler();
  else
    return -1;
	while (1)
	{

	}
}

/**
  * @brief  BSP硬件初始化函数
  * @param  无  
  * @retval 无
  */
static void BSP_init(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4);
	LED_GPIO_Config();
	//LED1_ON;
	USART_Config();
}
/**
  * @brief  任务创建函数，用于创建任务
  * @param  无  
  * @retval 无
  */
static void AppTaskCreate_Task(void * parameter)
{
	  BaseType_t xReturn =pdPASS;
		taskENTER_CRITICAL();
		xReturn=xTaskCreate((TaskFunction_t)LED1_Task,
										                  (const char *)"LED_Task",
                                      (uint32_t)512,
                                      (void *)NULL,
                                      (UBaseType_t)4,
                                      (TaskHandle_t *)&LED1_Task_Handle);
    if(xReturn==pdPASS)
      printf("LED任务创建成功\n");

    
    vTaskDelete(AppTaskCreate_Task_Handle);
    taskEXIT_CRITICAL();

}
/**
  * @brief  实现LED翻转的任务  
  * @param  无  
  * @retval 无
  */
static void LED1_Task(void * parameter)
{
	for(; ;)
	{
		LED1_ON;
		vTaskDelay(500);//延时500个tick
		printf("LED1 ON\n");
		LED1_OFF;
		vTaskDelay(500);//延时500个tick
    printf("LED1 OFF\n");
	}
}



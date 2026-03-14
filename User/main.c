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
static TaskHandle_t LED_Task_Handle;


/*内核对象句柄*/

/*全局变量声明*/
//AppTaskCreate 任务控制块
static StaticTask_t AppTaskCreate_TCB;
//LED_Task 任务控制块
static StaticTask_t LED_Task_TCB;
//AppTaskCreate 任务堆栈
static StackType_t AppTaskCreate_Stack[128];
//LED_Task 任务堆栈
static StackType_t LED_Task_Stack[128];
//空闲任务 任务堆栈
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
//定时器任务 任务堆栈
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
//空闲任务 任务控制块
static StaticTask_t IDLE_Task_TCB;
//定时器任务 任务控制块
static StaticTask_t Timer_Task_TCB;
/*函数声明*/
static void BSP_init(void);
static void AppTaskCreate_Task(void * parameter);
static void LED_Task(void * parameter);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                   StackType_t **ppxTimerTaskStackBuffer,
                                  uint32_t *puTimerTaskStackSize);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                  uint32_t *puIdleTaskStackSize);
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	BSP_init();//BSP硬件初始化
  AppTaskCreate_Task_Handle=xTaskCreateStatic((TaskFunction_t)AppTaskCreate_Task,
										                          (const char *)"AppTaskCreate_Task",
                                              (uint32_t)128,
                                              (void *)NULL,
                                              (UBaseType_t)3,
                                              (StackType_t *)AppTaskCreate_Stack,
                                              (StaticTask_t *)&AppTaskCreate_TCB);
  if(AppTaskCreate_Task_Handle!=NULL)
    vTaskStartScheduler();
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
		taskENTER_CRITICAL();
		LED_Task_Handle=xTaskCreateStatic((TaskFunction_t)LED_Task,
										                  (const char *)"LED_Task",
                                      (uint32_t)128,
                                      (void *)NULL,
                                      (UBaseType_t)4,
                                      (StackType_t *)LED_Task_Stack,
                                      (StaticTask_t *)&LED_Task_TCB);
    if(LED_Task_Handle != NULL)
      printf("LED任务创建成功\n");
    else
      printf("LED任务创建失败\n");
    
    vTaskDelete(AppTaskCreate_Task_Handle);
    taskEXIT_CRITICAL();

}
/**
  * @brief  实现LED翻转的任务  
  * @param  无  
  * @retval 无
  */
static void LED_Task(void * parameter)
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
/**
  * @brief  获取空闲任务的控制块内存和堆栈内存
  * @param  ppxIdleTaskTCBBuffer :任务控制块内存
  * @param  ppxIdleTaskStackBuffer :任务堆栈内存
  * @param  puIdleTaskStackSize  :任务堆栈大小
  * @retval 无
  */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                  uint32_t *puIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer=&IDLE_Task_TCB;
  *ppxIdleTaskStackBuffer=Idle_Task_Stack;
  *puIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}

/**
  * @brief  获取定时器任务的控制块内存和堆栈内存
  * @param  ppxIdleTaskTCBBuffer :任务控制块内存
  * @param  ppxIdleTaskStackBuffer :任务堆栈内存
  * @param  puIdleTaskStackSize  :任务堆栈大小
  * @retval 无
  */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                   StackType_t **ppxTimerTaskStackBuffer,
                                  uint32_t *puTimerTaskStackSize)
{
  *ppxTimerTaskTCBBuffer=&Timer_Task_TCB;
  *ppxTimerTaskStackBuffer=Timer_Task_Stack;
  *puTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
}



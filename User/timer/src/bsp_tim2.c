#include "bsp_tim2.h"

volatile uint32_t uwTick = 0;

void TIM2_Init_xms(uint8_t x)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 开时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. 定时器配置
    // 自动重装载值
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;  
    // 预分频器 72M / 72 = 1MHz → 1us 计数
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; 
    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 3. 开更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    // 4. NVIC 中断配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief  TIM2 中断服务函数 1ms 进一次
 */
void TIM2_IRQHandler(void)
{
    // 判断是否是更新中断
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        uwTick++;  // 1ms 计时+1
		lv_tick_inc(1);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除标志
    }
}
void delay_ms(uint32_t ms)
{
    uint32_t start = uwTick;
    while(uwTick - start < ms);
}


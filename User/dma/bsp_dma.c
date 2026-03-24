#include "./dma/bsp_dma.h"

void USART3_DMA_Config(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitTypeDef DMA_Initstructure;
    DMA_DeInit(IMAGE_DMA_CHANNLE);
    DMA_Initstructure.DMA_BufferSize=IMAGE_DMA_BUFFERSIZE;
    DMA_Initstructure.DMA_DIR=IMAGE_DMA_DIR;
    DMA_Initstructure.DMA_M2M=IMAGE_DMA_M2M;
    DMA_Initstructure.DMA_MemoryBaseAddr=(uint32_t)IMAGE_DMA_MEMORYBASEADDR;
    DMA_Initstructure.DMA_MemoryDataSize=IMAGE_DMA_MEMORYDATASIZE;
    DMA_Initstructure.DMA_MemoryInc=IMAGE_DMA_MEMORYINC;
    DMA_Initstructure.DMA_Mode=IMAGE_DMA_MODE;
    DMA_Initstructure.DMA_PeripheralBaseAddr=IMAGE_DMA_PERIPHERABASEADDR;
    DMA_Initstructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
    DMA_Initstructure.DMA_PeripheralInc=IMAGE_DMA_PERIPHERAINC;
    DMA_Initstructure.DMA_Priority=IMAGE_DMA_PRIORITY;
    DMA_Init(IMAGE_DMA_CHANNLE,&DMA_Initstructure);
	DMA_ClearFlag(DMA1_FLAG_GL4 | DMA1_FLAG_TC4 | DMA1_FLAG_HT4 | DMA1_FLAG_TE4);
    DMA_ITConfig(IMAGE_DMA_CHANNLE,DMA_IT_TC,ENABLE);
        NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}








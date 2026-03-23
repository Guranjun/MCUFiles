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
    //DMA_ITConfig(IMAGE_DMA_CHANNLE,DMA_IT_TC,ENABLE);
    
}








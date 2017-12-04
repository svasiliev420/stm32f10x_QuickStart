/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************

*/
    #include "usart.h"
    
    #include "stm32f1xx.h"
    #include "stm32f1xx_ll_bus.h"
    #include "stm32f1xx_ll_usart.h"
    #include "stm32f1xx_ll_rcc.h"
    #include "stm32f1xx_ll_gpio.h"
    #include "stm32f1xx_ll_dma.h"
    
    uint8_t usart_TX_frame32 [32];
    uint8_t usart_RX_frame32 [32];
    
    void USART_DMA_Flush()
    {
        LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_4);
        LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_5);
    }
    
/////
///// CONFIGURE USART
/////
    void INIT_USART_CLI420()
    {

/////
// GPIO
/////
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
        
        LL_GPIO_SetPinMode  (GPIOA, LL_GPIO_PIN_9,  LL_GPIO_MODE_OUTPUT|LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetPinSpeed (GPIOA, LL_GPIO_PIN_9,  LL_GPIO_SPEED_FREQ_LOW);
        LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_OUTPUT_);
        
        LL_GPIO_SetPinMode  (GPIOA, LL_GPIO_PIN_10,  LL_GPIO_MODE_INPUT|LL_GPIO_MODE_FLOATING);
        LL_GPIO_SetPinSpeed (GPIOA, LL_GPIO_PIN_10,  LL_GPIO_SPEED_FREQ_LOW);
        
/////
// DMA1_CH4 >> TX
/////
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
        LL_DMA_InitTypeDef _dma;
        

        LL_DMA_StructInit(&_dma);
            _dma.Mode                   = LL_DMA_MODE_NORMAL;
            _dma.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
            
            _dma.MemoryOrM2MDstAddress  = (uint32_t)&usart_TX_frame32;
            _dma.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
            _dma.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
            
            _dma.PeriphOrM2MSrcAddress  = (uint32_t)&(USART1->DR);
            _dma.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
            _dma.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
            
            _dma.NbData                 = 32;
        LL_DMA_Init (DMA1,LL_DMA_CHANNEL_4, &_dma);

        LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
        LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_4);
        NVIC_EnableIRQ  (DMA1_Channel4_IRQn);
        LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_4);
///////
// DMA1_CH5_ << RX
///////

        LL_DMA_StructInit(&_dma);
            _dma.Mode                   = LL_DMA_MODE_CIRCULAR;
            _dma.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
            
            _dma.PeriphOrM2MSrcAddress  = (uint32_t)&(USART1->DR);
            _dma.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
            _dma.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
            
            _dma.MemoryOrM2MDstAddress  = (uint32_t)&usart_RX_frame32;
            _dma.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
            _dma.MemoryOrM2MDstDataSize = LL_DMA_PDATAALIGN_BYTE;
            
            _dma.NbData                 = 32;
            _dma.Priority               = LL_DMA_PRIORITY_LOW;
            
            
        LL_DMA_Init       (DMA1,LL_DMA_CHANNEL_5, &_dma);
        LL_DMA_EnableIT_TC(DMA1,LL_DMA_CHANNEL_5);
        LL_DMA_EnableIT_HT(DMA1,LL_DMA_CHANNEL_5);
        NVIC_EnableIRQ(DMA1_Channel5_IRQn);

/////
// USART
/////
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1); 
        
        LL_USART_InitTypeDef   _usart;
        
        LL_USART_StructInit(&_usart);
            _usart.BaudRate             = 115200;
            _usart.DataWidth            = LL_USART_DATAWIDTH_8B;
            _usart.StopBits             = LL_USART_STOPBITS_1; 
            _usart.Parity               = LL_USART_PARITY_NONE;
            _usart.HardwareFlowControl  = LL_USART_HWCONTROL_NONE; 
            _usart.TransferDirection    = LL_USART_DIRECTION_TX_RX;
        LL_USART_Init(USART1,&_usart);
        
        LL_USART_EnableDMAReq_RX    (USART1);
        LL_USART_EnableDMAReq_TX    (USART1);
        LL_USART_EnableDirectionTx  (USART1);
        LL_USART_EnableDirectionRx  (USART1);
        
        LL_USART_Enable(USART1);
        NVIC_EnableIRQ(USART1_IRQn);

    }
    
    // USART1_RX_HANDLER
    //
    void DMA1_Channel5_IRQHandler (void)
    {
        if(DMA1->ISR & DMA_ISR_TCIF5) // Если обмен завершен 
        {
            uint32_t ref = *(uint32_t *)usart_RX_frame32;   // Берем первые 4 байта
            //uint32_t crc = CalcCRC32( (unsigned char*)usart_RX_frame32+4,28);
            
            //if( crc ^ ref == 0 ) //0xFFFFFFFFul;
            //{
                //hostAPI_exec_frame32( usart_RX_frame32 );
                //_radio_transmiter();
            //}
        }
        if(DMA1->ISR & DMA_ISR_HTIF5) { }// Если передана половина буфера  
        if(DMA1->ISR & DMA_ISR_TEIF5) { }// Если произошла ошибка при обмене
        
        LL_DMA_ClearFlag_GI5(DMA1);
    }
    //
    // USART1_TX_HANDLER
    //
    void DMA1_Channel4_IRQHandler(void)
    {
        if(DMA1->ISR & DMA_ISR_TCIF4) //.Если обмен завершен
        {
            LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_4);
            LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_4,32);
        } 
        if(DMA1->ISR & DMA_ISR_HTIF4) { } //.Если передана половина буфера
        if(DMA1->ISR & DMA_ISR_TEIF4) { } //.Если произошла ошибка при обмене
         
         LL_DMA_ClearFlag_GI4(DMA1);
    }
    
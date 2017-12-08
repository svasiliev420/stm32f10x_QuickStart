/**
  ******************************************************************************
  * @date    6/11/2017
  * @author  
  * @version 
  * @brief   
  ******************************************************************************

*/
    #include "header.h"
    
    const uint16_t __SIZE_FRAME = sizeof(STRUCT_FRAME32);
    
//////////////////////////
/// DEVICE HARDWARE
/////////////////////////
    USART_TypeDef*  _xusart      = USART1;
    
    DMA_TypeDef*    _xdma        = DMA1;
    uint32_t        _xdma_chTX   = LL_DMA_CHANNEL_4;
    uint32_t        _xdma_chRX   = LL_DMA_CHANNEL_5;
    
    GPIO_TypeDef*   _xgpio       = GPIOA;
    uint32_t        _xgpio_pinTX = LL_GPIO_PIN_9;
    uint32_t        _xgpio_pinRX = LL_GPIO_PIN_10;
    
//////////////////////////
/// DEVICE ATTRIBUTEs
/////////////////////////
    uint8_t  device_use_CRC         = 1;
    
    uint8_t  device_bufferTx[__SIZE_FRAME+32*6];
    uint8_t  device_bufferRx[__SIZE_FRAME+32*6];
    
    STRUCT_FRAME32* tx = (STRUCT_FRAME32*)&device_bufferTx;
    STRUCT_FRAME32* rx = (STRUCT_FRAME32*)&device_bufferRx;
    
    
    
/////////////////////////
// DEVICE METHODs
/////////////////////////    
    void INTERCOM_Send(void *_myData, uint16_t _mySize)
    {
        tx->crc     = 0xFFFFFFFF;             
        tx->dsize   = _mySize + __SIZE_FRAME-2; 
        tx->dorigin = NULL;                     
        
        memcpy( &tx->dorigin, _myData, _mySize);
        
        LL_DMA_SetDataLength (_xdma, _xdma_chTX, tx->dsize );
        LL_DMA_EnableChannel (_xdma, _xdma_chTX);
    }
    
    void INTERCOM_Setup()
    {
// GPIO
        LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_AFIO );
        LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_GPIOA );
        
        LL_GPIO_SetPinMode  (_xgpio, _xgpio_pinTX,  LL_GPIO_MODE_ALTERNATE );
        LL_GPIO_SetPinSpeed (_xgpio, _xgpio_pinTX,  LL_GPIO_SPEED_FREQ_LOW );
        LL_GPIO_SetPinPull  (_xgpio, _xgpio_pinRX,  LL_GPIO_PULL_UP );
        
        LL_GPIO_SetPinMode  (_xgpio, _xgpio_pinRX,  LL_GPIO_MODE_FLOATING );
        LL_GPIO_SetPinSpeed (_xgpio, _xgpio_pinRX,  LL_GPIO_SPEED_FREQ_LOW );
        LL_GPIO_SetPinPull  (_xgpio, _xgpio_pinRX,  LL_GPIO_PULL_UP );
        
// DMA 
        // MEMORY TO PERIPH
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
        
        LL_DMA_Init (_xdma,_xdma_chTX, &(LL_DMA_InitTypeDef)
        {
             .Priority               = LL_DMA_PRIORITY_LOW
             
            ,.Mode                   = LL_DMA_MODE_NORMAL
            ,.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH
            
            ,.MemoryOrM2MDstAddress  = (uint32_t)&device_bufferTx
            ,.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT
            ,.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE
            
            ,.PeriphOrM2MSrcAddress  = (uint32_t)&USART1->DR
            ,.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT
            ,.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE
            
            ,.NbData                 = 32
        });
        
        LL_DMA_EnableIT_TC  ( _xdma, _xdma_chTX );
        LL_DMA_EnableIT_HT  ( _xdma, _xdma_chTX );
        NVIC_EnableIRQ      (DMA1_Channel4_IRQn);
        
        // PERIH TO MEMORY
        LL_DMA_Init ( DMA1, LL_DMA_CHANNEL_5, &(LL_DMA_InitTypeDef)
        {
             .Priority               = LL_DMA_PRIORITY_LOW
            
            ,.Mode                   = LL_DMA_MODE_CIRCULAR
            ,.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY
            
            ,.PeriphOrM2MSrcAddress  = (uint32_t)&USART1->DR
            ,.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT
            ,.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE
            
            ,.MemoryOrM2MDstAddress  = (uint32_t)&device_bufferRx
            ,.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT
            ,.MemoryOrM2MDstDataSize = LL_DMA_PDATAALIGN_BYTE
            
            ,.NbData                 = 32
        });
        
        LL_DMA_EnableIT_TC( _xdma, _xdma_chRX );
        LL_DMA_EnableIT_HT( _xdma, _xdma_chRX );
        NVIC_EnableIRQ( DMA1_Channel5_IRQn );
        
        LL_DMA_EnableChannel(_xdma,_xdma_chRX);
        
// USART
        LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_USART1 );
        
        LL_USART_Init(USART1, &(LL_USART_InitTypeDef)
        {
             .BaudRate            = 115200
            ,.DataWidth           = LL_USART_DATAWIDTH_8B
            ,.StopBits            = LL_USART_STOPBITS_1 
            ,.Parity              = LL_USART_PARITY_NONE
            ,.HardwareFlowControl = LL_USART_HWCONTROL_NONE 
            ,.TransferDirection   = LL_USART_DIRECTION_TX_RX
        });
        
        LL_USART_EnableDMAReq_RX    (_xusart);
        LL_USART_EnableDMAReq_TX    (_xusart);
        LL_USART_EnableDirectionTx  (_xusart);
        LL_USART_EnableDirectionRx  (_xusart);
        NVIC_EnableIRQ(USART1_IRQn);
        
        LL_USART_Enable(_xusart);
    }
    
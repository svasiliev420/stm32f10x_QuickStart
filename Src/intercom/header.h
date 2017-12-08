#ifndef __STM32_DEVICE_INTERCOM
#define __STM32_DEVICE_INTERCOM
    
    #include "stdio.h"
    
    #include "stm32f1xx.h"
    #include "stm32f1xx_ll_bus.h"
    #include "stm32f1xx_ll_usart.h"
    #include "stm32f1xx_ll_rcc.h"
    #include "stm32f1xx_ll_gpio.h"
    #include "stm32f1xx_ll_dma.h"
    
    
    typedef struct
    {
        uint32_t crc;
        uint16_t dsize;
        uint8_t  dorigin; //pointer
    }STRUCT_FRAME32;
    
    void INTERCOM_Send(void *,uint16_t);
    void INTERCOM_Setup();
     
    extern uint8_t  device_bufferTx[];
    extern uint8_t  device_bufferRx[];
    extern uint8_t  device_use_CRC;
    
    extern USART_TypeDef*  _xusart;
    extern DMA_TypeDef*    _xdma;
    extern uint32_t        _xdma_chTX;
    extern uint32_t        _xdma_chRX;
    extern GPIO_TypeDef*   _xgpio;
    extern uint32_t        _xgpio_pinTX;
    extern uint32_t        _xgpio_pinRX;
    
#endif

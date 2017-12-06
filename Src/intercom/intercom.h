#ifndef __DEVICE_INTERCOM
#define __DEVICE_INTERCOM
    
    #include "stdio.h"
    
    #include "stm32f1xx.h"
    #include "stm32f1xx_ll_bus.h"
    #include "stm32f1xx_ll_usart.h"
    #include "stm32f1xx_ll_rcc.h"
    #include "stm32f1xx_ll_gpio.h"
    #include "stm32f1xx_ll_dma.h"
    
    void INTERCOM_Send(void *);
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

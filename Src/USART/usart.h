#ifndef STM32_MOD_USART
#define STM32_MOD_USART
    
    void INIT_USART_CLI420();
    void USART_DMA_Flush();
     
    extern unsigned char    usart_RX_frame32[];
    extern unsigned char    usart_TX_frame32[];
    
#endif

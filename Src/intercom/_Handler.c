/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************

*/
    #include "intercom.h"
    
    #include "../crc/crc.h"
    #include "../hostapi/host.h"
    
    //
    // INTERCOM SEND MESSAGE
    //
    void DMA1_Channel4_IRQHandler(void)
    {
        if(_xdma->ISR & DMA_ISR_TCIF4) //.Если обмен завершен
        {
            LL_DMA_DisableChannel(_xdma, _xdma_chTX);
            LL_DMA_ClearFlag_TC4 (_xdma);
            
           //WRITE_REG(DMAx->IFCR, DMA_IFCR_CTCIF4);
        } 
        if(_xdma->ISR & DMA_ISR_HTIF4) //.Если передана половина буфера 
        { 
            LL_DMA_ClearFlag_HT4 (_xdma); 
        } 
        if(_xdma->ISR & DMA_ISR_TEIF4) { } //.Если произошла ошибка при обмене
         
    }
    
    //
    // INTERCOM RECIEVE MESSAGE
    //
    void DMA1_Channel5_IRQHandler (void)
    {
        if(_xdma->ISR & DMA_ISR_TCIF5) // Если обмен завершен 
        {
            if(device_use_CRC == 1)
            {
                uint32_t* ref    = (uint32_t *)&device_bufferRx + 0;
                uint8_t*  refData= (uint8_t*)ref + 4;
                uint32_t crc     = CRC32_Calc( refData, 32 - 4);
                if( crc^ref == 0 ) //0xFFFFFFFFul;
                {
                    host_Exec32( refData );
                }
            }
            
            LL_DMA_ClearFlag_TC5 (_xdma);
        }
        
        if(_xdma->ISR & DMA_ISR_HTIF5) { }// Если передана половина буфера  
        if(_xdma->ISR & DMA_ISR_TEIF5) { }// Если произошла ошибка при обмене
    }
    
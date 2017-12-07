/**
  ******************************************************************************
  * @file    
  * @author  
  * @version V0.0.0
  * @date    11-Aug-2017
  * @brief    
  ******************************************************************************

*/
    #include "../intercom/header.h"
    #include "core.h"

    ///
    /// 
    /// 
    /// 00-00 00-00-00-00 00-00
    ///  ^           ^     ^
    int hostapi_MCU_MEMREAD(uint8_t* _packRX)
    {
        _packRX+=2; uint32_t _baseAddr = *(uint32_t*)_packRX;
        _packRX+=5; uint8_t  _size     = *_packRX;
        
        INTERCOM_Send( (uint32_t*)(_baseAddr), _size );
        
        return 8+1;
    }
    /// 
    /// 00-00 00-00-00-00 00-00
    ///  ^           ^     ^
    int hostapi_MCU_MEMWRITE(uint8_t *ptr_frame)
    {
        //memcpy(usart_TX_frame32, &usart_RX_frame32, 31);
        return 1;
    }
    

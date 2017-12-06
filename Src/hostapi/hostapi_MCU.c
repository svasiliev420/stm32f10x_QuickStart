/**
  ******************************************************************************
  * @file    
  * @author  
  * @version V0.0.0
  * @date    11-Aug-2017
  * @brief    
  ******************************************************************************
    


*/

#include "host.h"
#include "host_opcode.h"

#include "../intercom/intercom.h"

    /// 
    /// 00-00 00-00-00-00 00-00
    ///  ^           ^     ^
    int hostapi_MCU_MEMREAD(uint8_t* _ptr)
    {
        _ptr+=2;
        volatile uint32_t _baseAddr = *(uint32_t*)_ptr;
        _ptr+=5;
        volatile uint8_t  _amout    =*_ptr;
        
        INTERCOM_Send( (uint32_t*)(_baseAddr) );
        
        return 8+1;
    }
    /// 
    /// 00-00-00-00
    ///
    int hostapi_MCU_MEMWRITE(uint8_t *ptr_frame)
    {
        //memcpy(usart_TX_frame32, &usart_RX_frame32, 31);
        return 1;
    }
    

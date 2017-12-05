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


    int hostapi_MCU_MEMREAD(uint8_t* _ptr)
    {
        volatile uint32_t _baseAddr =*((uint32_t*)_ptr + 0);
        volatile uint8_t  _amout    =*((uint8_t* )_ptr + 8);
        
        //memcpy(usart_TX_frame32, &usart_RX_frame32, 31);
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
    



int hostapi_VM_PING(char *ptr_frame)
{
    //memset(usart_TX_frame32, 0, 31);

    //SART_DMA_Flush();
    
    return 1;
}


int hostapi_VM_TERMINATE(uint32_t *ptr_frame)
{
    return 1;
}

int hostapi_VM_RESTART(uint32_t *ptr_frame)
{
    return 1;
}

///
/// 00-00-00-00
///
int hostapi_VM_MEMREAD(uint32_t *ptr_frame)
{
    //usart_TX_frame32[0]= PROTO_CONNECT;
    return 1;
}

int hostapi_VM_MEMWRITE(uint32_t *ptr_frame)
{
    return 1;
}

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
    
    
    /// PROTO_SYNC
    /// size: 00-00-00-00
    ///

    
    ///
    /// 00-00-00-00
    ///
    void get_uid(void)
    {
        int i = 0;
        extern  volatile uint32_t Unique_ID_Low;
        extern  volatile uint32_t Unique_ID_Mid;
        extern  volatile uint32_t Unique_ID_Hig;
        
        //usart_TX_buff(i++, Unique_ID_Low>>24);
        //usart_TX_buff(i++, Unique_ID_Low>>16);
        //usart_TX_buff(i++, Unique_ID_Low>>8);
        //usart_TX_buff(i++, Unique_ID_Low>>0);
        //usart_TX_buff(i++, Unique_ID_Mid>>24);
        //usart_TX_buff(i++, Unique_ID_Mid>>16);
        //usart_TX_buff(i++, Unique_ID_Mid>>8);
        //usart_TX_buff(i++, Unique_ID_Mid>>0);
        //usart_TX_buff(i++, Unique_ID_Hig>>24);
        //usart_TX_buff(i++, Unique_ID_Hig>>16);
        //usart_TX_buff(i++, Unique_ID_Hig>>8);
        //usart_TX_buff(i++, Unique_ID_Hig>>0);
        //usart_TX_buff(i++, END);
    }

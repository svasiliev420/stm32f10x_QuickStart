/**
  ******************************************************************************
  * @file    
  * @author  
  * @version V0.0.0
  * @date    08.08.2017
  * @brief   
  ******************************************************************************


*/
#ifndef __HOSTAPI_CORE
#define __HOSTAPI_CORE

    #include "stdint.h"
    #include "stdio.h"
    
    extern unsigned char   vm_stream_0 [32];
    extern unsigned char   vm_stream_1 [32];
    extern unsigned char   vm_stream_2 [32];
    
////
//// 
////
    typedef int (*API_PtrFunc)(char *);
    
    typedef struct
    {
        API_PtrFunc ep;
        unsigned int        tick;
        
    }STRUCT_HOST_FUNCTION;

    extern STRUCT_HOST_FUNCTION   *vm_tableFunction[255];
    
    extern void host_Bind      ( uint8_t,API_PtrFunc, uint32_t );
    extern void host_Clear     ();
    extern int  host_Exec      ( uint8_t ,uint8_t*);
    extern void host_Exec32    ( uint8_t* );

    
    extern int hostapi_MCU_MEMREAD (uint8_t*);
    extern int hostapi_MCU_MEMWRITE(uint8_t*);
        
/////
///// 
/////
    extern int  vm_check_end_programm();
    extern int  vm_check_end_opcode();
    
    extern void vm_execute();
    extern void vm_set_stream(char*);
    extern void vm_next_opcode();
    extern void vm_read_opcode();
    
    extern void sys_goto();
    extern void sys_pause();
    extern void sys_stop();
    extern void sys_restart();
    extern void sys_terminate();
    
#endif

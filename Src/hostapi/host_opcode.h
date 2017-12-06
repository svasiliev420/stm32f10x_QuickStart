/**
  ******************************************************************************
  * @file    
  * @author  
  * @version V0.0.2
  * @date    09-Sep-2017
  * @brief   Opcodes for Machine 
  ******************************************************************************

*/

#ifndef __HOSTAPI_OPCODE
#define __HOSTAPI_OPCODE

    #define __RESERVED      0x00
    #define __PROTO         0xA0
    #define __VM            0xB0
    #define __CG          	0xC0
    #define __DATA          0xD0
    #define __USER          0xF0

    #define NOP             __RESERVED+0x00
    #define END             __RESERVED+0x0D

    #define PROTO_CONNECT   __PROTO+0x0A   // Начало работы с STM32
    #define PROTO_ECHO      __PROTO+0x0B
    #define PROTO_MODE      __PROTO+0x0C
    #define PROTO_SYNC      __PROTO+0x0E
    
    #define VM_STREAM    		__VM+0x01
    #define VM_PAUSE      	__VM+0x02
    #define VM_RESTART     	__VM+0x03
    #define VM_TERMINATE    __VM+0x04
    #define VM_MEM_READ     __VM+0x05
    #define VM_MEM_WRITE    __VM+0x06
    
    #define CG_FONT         __CG+0x01 
    #define CG_BITBLT       __CG+0x02
    #define CG_CLEAR        __CG+0x03
    #define CG_LINE         __CG+0x04
    #define CG_RECT         __CG+0x05
    #define CG_INIT         __CG+0x0A
    #define CG_INFO         __CG+0x0B
    
    #define DATA_OK         __DATA+0x1
    #define DATA_LOG        __DATA+0x2
    #define DATA_ERROR      __DATA+0x3
    #define DATA_LED        __DATA+0x4
    #define DATA_LIGHT      __DATA+0x5
    #define DATA_BUTTON     __DATA+0x6

    #define MEMREAD          __USER+0x01
    #define MEMWRITE         __USER+0x02
    #define USER_3           __USER+0x03

    extern int  hostapi_PROTO_MODE    (char*);
    extern int  hostapi_PROTO_CONNECT (char*);
    extern int  hostapi_PROTO_ECHO    (char*);
    extern int  hostapi_PROTO_SYNC    (char *ptr_frame);

    extern char hostapi_CG_BITBLT   	();
    extern int  hostapi_CG_CLEAR    	();
    extern char hostapi_CG_FONT     	();
    extern int  hostapi_CG_INIT     	(char *);
    extern int  hostapi_CG_INFO     	(char *);

    extern int  hostapi_VM_PING      	(char *);
    extern int  hostapi_VM_TERMINATE 	();
    extern int  hostapi_VM_RESTART   	();
    extern int  hostapi_VM_MEMREAD   	();
    extern int  hostapi_VM_MEMWRITE  	();

#endif
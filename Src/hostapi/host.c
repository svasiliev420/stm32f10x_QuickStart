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

    
    

    unsigned char   vm_stream_1 [32];
    unsigned char   vm_stream_2 [32];
    
	/// 
	/// Разряженый массив
	/// http://cpp.com.ru/shildt_spr_po_c/23/2304.html
	STRUCT_HOST_FUNCTION   *vm_tableFunction[255];

	/// 
	/// 
	/// 
    void host_Clear()
    {
        for(register uint8_t i = 0; i!=255; i++) 
            vm_tableFunction[i] = 0;
    }
	/// 
	/// 
	/// 
	void host_Bind(uint8_t cmd, API_PtrFunc ptr, uint32_t time)
	{
		// Создаем структуры и сохраняем указатель в таблице    
		// TODO: освобождение памяти free(ptr);
		vm_tableFunction[cmd]       = (STRUCT_HOST_FUNCTION*)malloc(sizeof(STRUCT_HOST_FUNCTION));
		vm_tableFunction[cmd]->ep   = ptr;
		vm_tableFunction[cmd]->tick = time;
	}
    ///
    /// Парсер
    ///
    void host_Exec32 (uint8_t* _ptr)
    {
        uint8_t _cursor  = 0;
        uint8_t _opcode  = 0;
        
        while(_cursor < 32-4)
        {            
            _opcode = *(_ptr + _cursor);
            
            _cursor = _cursor + host_Exec(_opcode, _ptr);
        }
        
    }
    
    ///
    ///
    ///
    int host_Exec(uint8_t cmd, uint8_t* ptr_frame) // ...arg
    {
        if( cmd <= 255 && vm_tableFunction[cmd] !=0 && vm_tableFunction[cmd]->ep != 0)
        {
            return vm_tableFunction[cmd]->ep(ptr_frame);
        }
         
        return 1; // hack for frame32 cursor
    }

    
    STRUCT_HOST_FUNCTION hostapi_Check(char opcode)
    {
        
    }
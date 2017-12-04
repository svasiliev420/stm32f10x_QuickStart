#include "misc.h"
#include "crc.h"


uint32_t  revbit(uint32_t  Data)
{
        __RBIT(Data);
        //asm("rbit r0,r0");  // Аргумент передаётся через r0. Над ним и выполнить команду rbit
        return Data;
}
//https://www.keil.com/pack/doc/CMSIS/Core/html/group__intrinsic__CPU__gr.html#ga4717abc17af5ba29b1e4c055e0a0d9b8
unsigned int CalcCRC32(unsigned char *Buf, unsigned int Len)
{
        unsigned int i;
        unsigned int Temp;
        
        RCC->AHBENR |= RCC_AHBENR_CRCEN; //Разрешить тактирование CRC-юнита
        
        CRC->CR = 1;
        __NOP(); //Аппаратная готовность за 4 такта, жду...
        __NOP();
        __NOP();
        
        // Аппаратный CRC-расчёт работает с 32-битными словами. Т.е. сразу по 4 байта из входной последовательности
        i = Len >> 2;
        while(i--)
        {
            Temp = *((unsigned int*)Buf);
            Temp = revbit(Temp); // Переставить биты во входных данных
            CRC->DR = Temp;
            
            Buf += 4;
        }
        
        Temp = CRC->DR;
        //Temp = revbit(Temp); //Переставить биты в выходных данных
        //Temp ^= 0xFFFFFFFFul;
        return Temp;
        // Обработать оставшиеся байты (классическим не аппаратным методом), если их число не было кратно 4
        //i = Len & 3;
}



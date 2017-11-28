#include "stm32f10x.h"
#include <stdint.h>
#include <string.h>

char USART1_putc(const char);
int  USART1_puts(const char *);
void delay(uint32_t);

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC, &(GPIO_InitTypeDef)
	{
		.GPIO_Pin	= GPIO_Pin_9| GPIO_Pin_8,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_Mode	= GPIO_Mode_Out_PP
	});
	
	for(;;)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, !GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9));
		delay(25 * 2048);
	}

	return 0;
}

char USART1_putc(const char ch)
{
	uint32_t cnt = 100000;

	USART_SendData(USART1, (uint8_t) ch );

	while ( (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) && cnt-- );

	return (0 == cnt) ? -1 : ch;
}

int USART1_puts(const char * str)
{
	int r	= -1;
	int size= 0;

	uint8_t i;

	if (str == NULL)
		return r;

	size = strlen(str);

	for (i = 0; i < size; i++)
	{
		 if ( USART1_putc( *(str + i) ) == -1 )
			 return r;
	}

	r = 0;
	return r;
}
void delay(uint32_t time_delay)
{
	uint32_t i;
	for (i = 0; i < time_delay; i++);
}

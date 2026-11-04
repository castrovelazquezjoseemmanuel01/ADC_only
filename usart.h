#include <stm32f10x.h>
#ifndef _usart
#define _usart
	void init_usart1_pin(void);
	void init_usart1(void);
	void usart1_sendbyte(unsigned char c);
	void usart1_sendstring(char s[]);
	uint8_t usart1_receivebyte();
#endif
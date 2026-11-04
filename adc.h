#include <stm32f10x.h>
#ifndef _adc
#define _adc
	void init_adc1_pin(void);
	void init_adc1(void);
	uint16_t adc1_read(void);
#endif
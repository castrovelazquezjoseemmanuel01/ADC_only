#include <stm32f10x.h>
#include "adc.h"

void init_adc1_pin(void) {
    RCC->APB2ENR |= (1U << 2) | (1U << 9); /* GPIOA y ADC1 */
    GPIOA->CRL &= ~(0xFU << 4);            /* PA1 analogico: MODE=CNF=0 */
}

void init_adc1(void) {
    /* ADCCLK=PCLK2/6: maximo 12 MHz si PCLK2=72 MHz */
    RCC->CFGR = (RCC->CFGR & ~(3U << 14)) | (2U << 14);
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;
    ADC1->SMPR2 = (ADC1->SMPR2 & ~(7U << 3)) | (7U << 3); /* 239.5 ciclos */
    ADC1->SQR1 &= ~(0xFU << 20);           /* Una conversion regular */
    ADC1->SQR3 = (ADC1->SQR3 & ~0x1FU) | 1U; /* Canal 1 */
    ADC1->CR2 |= (7U << 17) | (1U << 20);  /* EXTSEL=111, EXTTRIG=1 */
    ADC1->CR2 |= 1U;                       /* ADON */
    { volatile uint32_t i; for (i = 0; i < 2000U; ++i) { __NOP(); } }
    ADC1->CR2 |= (1U << 3);               /* RSTCAL */
    while (ADC1->CR2 & (1U << 3)) { }
    ADC1->CR2 |= (1U << 2);               /* CAL */
    while (ADC1->CR2 & (1U << 2)) { }
}

uint16_t adc1_read(void) {
    ADC1->SR &= ~(1U << 1);
    ADC1->CR2 |= (1U << 22);              /* SWSTART */
    while ((ADC1->SR & (1U << 1)) == 0U) { }
    return (uint16_t)(ADC1->DR & 0x0FFFU);
}

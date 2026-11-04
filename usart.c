#include <stm32f10x.h>
#include "usart.h"

void init_usart1_pin(void) {
    RCC->APB2ENR |= (1U << 14) | (1U << 2) | (1U << 0);
    /* PA9 TX: alternate function push-pull 50 MHz (CNF=10, MODE=11) */
    GPIOA->CRH = (GPIOA->CRH & ~(0xFU << 4)) | (0xBU << 4);
    /* PA10 RX: entrada flotante (CNF=01, MODE=00) */
    GPIOA->CRH = (GPIOA->CRH & ~(0xFU << 8)) | (0x4U << 8);
}

void init_usart1(void) {
    uint32_t pclk2, prescaler;
    static const uint8_t apb_div[8] = {1,1,1,1,2,4,8,16};
    SystemCoreClockUpdate();
    prescaler = (RCC->CFGR >> 11) & 7U;
    pclk2 = SystemCoreClock / apb_div[prescaler];
    USART1->CR1 = 0;
    USART1->BRR = (pclk2 + 4800U) / 9600U; /* OVER8=0, USARTDIV x16 */
    USART1->CR1 = (1U << 13) | (1U << 3) | (1U << 2);
}

void usart1_sendbyte(unsigned char c) {
    while ((USART1->SR & (1U << 7)) == 0U) { } /* TXE */
    USART1->DR = c;
}

void usart1_sendstring(char s[]) {
    while (*s != '\0') { usart1_sendbyte((unsigned char)*s++); }
    while ((USART1->SR & (1U << 6)) == 0U) { } /* TC */
}

uint8_t usart1_receivebyte(void) {
    while ((USART1->SR & (1U << 5)) == 0U) { }
    return (uint8_t)USART1->DR;
}

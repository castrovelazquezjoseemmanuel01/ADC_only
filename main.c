#include <stm32f10x.h>
#include "leds.h"
#include "exti.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"

int main(void) {
    uint16_t value;
    uint32_t millivolts;
    char message[32];

    SystemCoreClockUpdate();
    init_led_pin();
    led_on_off();
    init_exti_pin();
    init_exti();
    init_usart1_pin();
    init_usart1();
    init_adc1_pin();
    init_adc1();

    usart1_sendstring("ADC1 PA1 listo\r\n");
    while (1) {
        value = adc1_read();
        millivolts = ((uint32_t)value * 3300UL + 2047UL) / 4095UL;
        /* Enviar texto decimal sin necesitar printf con soporte float */
        {
            uint32_t whole = millivolts / 1000UL;
            uint32_t frac = millivolts % 1000UL;
            message[0] = 'V'; message[1] = '=';
            message[2] = (char)('0' + whole); message[3] = '.';
            message[4] = (char)('0' + frac / 100UL);
            message[5] = (char)('0' + (frac / 10UL) % 10UL);
            message[6] = (char)('0' + frac % 10UL);
            message[7] = ' '; message[8] = 'V';
            message[9] = '\r'; message[10] = '\n'; message[11] = '\0';
        }
        usart1_sendstring(message);
        delay_ms(200); /* aproximado: depende del reloj/optimizacion */
    }
}
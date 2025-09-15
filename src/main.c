#include "motordriver.h"

uint16_t adcValue;

int main() {
    systemInit();
    while(1) {
        GPIOA->ODR ^= (1 << 5);            // Toggle onboard LED
        adcValue = ADCReadA0();
        usart2Printf("ADC Value: %d\r\n" , adcValue);
        delayMs(100);
    }
    return 0;
}
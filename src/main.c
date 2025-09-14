#include "stm32f4xx.h"
#include "usart.h"


void SysTickInit(void) { // Precise delay function using SysTick
    SysTick->LOAD = SystemCoreClock/1000 - 1;  // 1 ms period
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_ENABLE_Msk;
}

void delayMs(uint32_t ms) {
    uint32_t start = SysTick->VAL;
    for (uint32_t i = 0; i < ms; i++) {
    
        while (start <= SysTick->VAL);// Wait until the SysTick counter wraps around
        while (start > SysTick->VAL);
        start = SysTick->VAL;
    }
}

int main() {
    SysTickInit();
    usart2Init(115200);
    usart2Printf("USART2 Initialized!\r\n");
    usart2Printf("System clock: %d Hz\r\n", SystemCoreClock);
    usart2NewLine();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA
    GPIOA->MODER |= (1 << 10);             // PA5 as output
    uint32_t counter = 0;
    
    while(1) {
        GPIOA->ODR ^= (1 << 5);            // Toggle PA5
        counter++;
        usart2Printf("Counter: %d\r\n", counter);
        delayMs(500);
    }
    return 0;
}
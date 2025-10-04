#ifndef CONFIG
#define CONFIG

#include "stm32f4xx.h"

#define CMD_BUFFER_SIZE 30
#define SystemCoreClock 16000000 // 16MHz in datasheet

#define NUMREADINGS 15 // for averaging adc vals

#define ASCII_BACKSPACE 0x7F // used in puTTy's terminal
#define ASCII_ESC 0x1B
#define ASCII_OPENING_BRACKET 0x5B
#define ASCII_UP 0x41
#define ASCII_DOWN 0x42
#define ASCII_RIGHT 0x43
#define ASCII_LEFT 0x44

#define MIN_DEADBAND_THRESHOLD 1048 // adc values for deadband
#define MAX_DEADBAND_THRESHOLD 3048
#define MAX_LEFT 32 // top left spinning speed at adc value 35
#define MAX_RIGHT 4096 // top right spinning speed at adc value 4096

#define CLOCK_GPIOA RCC_AHB1ENR_GPIOAEN
#define CLOCK_GPIOB RCC_AHB1ENR_GPIOBEN
#define CLOCK_GPIOC RCC_AHB1ENR_GPIOCEN
#define CLOCK_GPIOD RCC_AHB1ENR_GPIODEN
#define CLOCK_GPIOE RCC_AHB1ENR_GPIOEEN

#define CLOCK_TIM2 RCC_APB1ENR_TIM2EN
#define CLOCK_TIM3 RCC_APB1ENR_TIM3EN
#define CLOCK_TIM4 RCC_APB1ENR_TIM4EN
#define CLOCK_TIM5 RCC_APB1ENR_TIM5EN




// Timing functions
void SysTickInit(void) { // for more accurate delay func
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

#endif
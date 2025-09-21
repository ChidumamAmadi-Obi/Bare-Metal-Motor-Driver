#ifndef CONFIG
#define CONFIG

#include "stm32f4xx.h"

#define CMD_BUFFER_SIZE 100
#define SystemCoreClock 16000000 // 16MHz in datasheet

#define SAMPLE_COUNT 10 // for averaging adc vals

#define ASCII_BACKSPACE 0x7F // used in puTTy's terminal
#define ASCII_ESC 0x1B
#define ASCII_OPENING_BRACKET 0x5B
#define ASCII_UP 0x41
#define ASCII_DOWN 0x42
#define ASCII_RIGHT 0x43
#define ASCII_LEFT 0x44

#define MIN_DEADBAND_THRESHOLD 1048
#define MAX_DEADBAND_THRESHOLD 3048

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
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "stm32f4xx.h"

#include <stdint.h>
#include <stdbool.h>
#include "usart.h"
#include "adc.h"
#include "gpio.h"

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

// Initlalize system
void systemInit(){
    SysTickInit();
    usart2Init(115200);
    usart2Printf("USART2 Initialized...\r\n");
    usart2Printf("System clock: %d Hz\r\n", SystemCoreClock);
    usart2NewLine();

    ADCInit();
    usart2Printf("ADC A0 Initialized...\r\n");
    usart2NewLine();

    enableClocks();
    usart2Printf("Clocks TIM2 and TIM3 configured...\r\n");
    usart2NewLine();

    configGPIO();
    usart2Printf("GPIO Pins D5 and D6 configured...\r\n");
    usart2NewLine();

    configureTimerPWM_D6(0); // D6 and D5 with TIM2 and TIM3 - 10Hz, 0% duty cycle
    configureTimerPWM_D5(0);
    usart2Printf("Starting D6 and D5 on 0 percent duty cycle...\r\n");
    usart2NewLine();
    configureOnBoardLED();
}

#endif
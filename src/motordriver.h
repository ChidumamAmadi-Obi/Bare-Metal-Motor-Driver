#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#define MIN_DEADBAND_THRESHOLD 1048
#define MAX_DEADBAND_THRESHOLD 3048

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

    ADCInit();
    usart2Printf("ADC A0 Initialized...\r\n");

    enableClocks();
    usart2Printf("Clocks TIM2 and TIM3 configured...\r\n");

    configGPIO();
    usart2Printf("GPIO Pins D5 and D6 configured...\r\n");

    configureTimerPWM_D6(0); // D6 and D5 with TIM2 and TIM3 - 10Hz, 0% duty cycle
    configureTimerPWM_D5(0);
    usart2Printf("Starting D6 and D5 on 0 percent duty cycle...\r\n");
    configureOnBoardLED();
}

// motor control
inline uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void motorControl(uint16_t adcValue){
    uint8_t motorSpeedL;
    uint8_t motorSpeedR;

    if (adcValue > MIN_DEADBAND_THRESHOLD && adcValue < MAX_DEADBAND_THRESHOLD){ // if adc in deadband
        motorSpeedR = 0;
        motorSpeedL = 0;
    } else {
        if ( adcValue < MIN_DEADBAND_THRESHOLD ) {
            motorSpeedL = map(adcValue, 0, MIN_DEADBAND_THRESHOLD, 255, 0);
            motorSpeedR = 0;
        }
        if ( adcValue > MAX_DEADBAND_THRESHOLD ) {
            motorSpeedR = map(adcValue, 4096, MAX_DEADBAND_THRESHOLD, 255, 0);
            motorSpeedL = 0;
        }
    }
    
    configureTimerPWM_D5(motorSpeedR);
    configureTimerPWM_D6(motorSpeedL);
    usart2Printf("ADC Value %d, Motor Speed %d %d\r\n",adcValue, motorSpeedL, motorSpeedR);
}

#endif
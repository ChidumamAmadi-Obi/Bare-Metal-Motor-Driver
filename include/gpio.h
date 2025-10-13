#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"

void enableClocks(AHB1_Periphs_t ahb1Periphs, APB1_Periphs_t apb1Periphs, APB2_Periphs_t apb2Periphs) { // enables clocks needed
    if (ahb1Periphs != 0) { // enables clocks from AHB1 peripherals
        RCC->AHB1ENR |= ahb1Periphs;
    }
    if (apb1Periphs != 0) { // enables clocks from APB1 peripherals
        RCC->APB1ENR |= apb1Periphs;
    }
    if (apb2Periphs != 0) { // enables clocks from APB2 peripherals
        RCC->APB2ENR |= apb2Periphs;
    }
}

void configGPIO_PWM(void){ // each PWM enabled GPIO pin is configured with a separate clock 
    // Configures D6 (PB10) with TIM2_CH3
    GPIOB->MODER &= ~GPIO_MODER_MODER10;       
    GPIOB->MODER |= GPIO_MODER_MODER10_1;      
    
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL10; // alternate function mode       
    GPIOB->AFR[1] |= (1 << 8);                 
    
    // Configures D5 (PB4) with TIM3_CH1
    GPIOB->MODER &= ~GPIO_MODER_MODER4;        
    GPIOB->MODER |= GPIO_MODER_MODER4_1;       
    
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4;        
    GPIOB->AFR[0] |= (2 << 16);        
}
void configTimerPWM_D6(uint8_t dutyCycle) {
    TIM2->CR1 &= ~TIM_CR1_CEN;                
    TIM2->PSC = 16 - 1;                       // 16MHz/16 = 1MHz timer clock
    TIM2->ARR = 400 - 1;                      // 40kHz/400 = 100Hz PWM frequency
    
    // Configure channel 3
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3M;           
    TIM2->CCMR2 |= (6 << TIM_CCMR2_OC3M_Pos); 
    TIM2->CCMR2 |= TIM_CCMR2_OC3PE;           
    TIM2->CCER |= TIM_CCER_CC3E;               
    TIM2->CCR3 = (400 * dutyCycle) / 100; // sets duty cycle                
    TIM2->CR1 |= TIM_CR1_ARPE;                 
    TIM2->CR1 |= TIM_CR1_CEN;                 
}
void configTimerPWM_D5(uint8_t dutyCycle) {
    TIM3->CR1 &= ~TIM_CR1_CEN;          
    TIM3->PSC = 16 - 1;                       
    TIM3->ARR = 400 - 1;                     
    
    // Configure channel 1
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;           
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);  
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;           
    TIM3->CCER |= TIM_CCER_CC1E;              
    TIM3->CCR1 = (400 * dutyCycle) / 100;                           
    TIM3->CR1 |= TIM_CR1_ARPE;                 
    TIM3->CR1 |= TIM_CR1_CEN;                 
}

void configGPIO_Port(GPIO_TypeDef* port, GPIOMode_t mode, GPIOOutputType_t outputType, GPIOSpeed_t speed) { // configures port a b or c at once
    for (uint8_t pin = 0; pin < 8; pin++) {
        port->MODER = (port->MODER & ~(0x3 << (pin * 2))) | ((mode & 0x3) << (pin * 2)); // Sets port as input or output
        
        if (mode == 1) { // Configure Output Type Register - only for output mode
            if (outputType) {
                port->OTYPER |= (1 << pin);  // Open-drain
            } else {
                port->OTYPER &= ~(1 << pin); // Push-pull
            }
        }
        
        if (mode == 1) { // Configure OSPEEDR (Output Speed Register) - only for output mode
            port->OSPEEDR = (port->OSPEEDR & ~(0x3 << (pin * 2))) | ((speed & 0x3) << (pin * 2));
        }
    }
}
void writeToPort(GPIO_TypeDef* port, uint8_t output) { // writes to port a b or c at once
    port->ODR = output;
}
void configureOnBoardLED(void){ 
    GPIOA->MODER |= (1 << 10);                 // Set PA5 as output
}

#endif
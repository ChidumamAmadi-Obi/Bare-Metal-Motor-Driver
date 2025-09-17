#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"

void enableClocks(void){
    // Clocks for PWM pins
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;       
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;       
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;        
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;        
}
void configGPIO(void){ // each GPIO pin is configured with a separate clock and set to alternate function
    // Configure D6 (PB10) as TIM2_CH3
    GPIOB->MODER &= ~GPIO_MODER_MODER10;       
    GPIOB->MODER |= GPIO_MODER_MODER10_1;      
    
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL10;       
    GPIOB->AFR[1] |= (1 << 8);                 
    
    // Configure D5 (PB4) as TIM3_CH1
    GPIOB->MODER &= ~GPIO_MODER_MODER4;        
    GPIOB->MODER |= GPIO_MODER_MODER4_1;       
    
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4;        
    GPIOB->AFR[0] |= (2 << 16);                
}

void configureTimerPWM_D6(uint8_t dutyCycle) {
    TIM2->CR1 &= ~TIM_CR1_CEN;                
    TIM2->PSC = 16 - 1;                       // 16MHz/16 = 1MHz timer clock
    TIM2->ARR = 400 - 1;                      // 40kHz/400 = 100Hz PWM frequency
    
    // Configure channel 3
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3M;           
    TIM2->CCMR2 |= (6 << TIM_CCMR2_OC3M_Pos); 
    TIM2->CCMR2 |= TIM_CCMR2_OC3PE;           
    TIM2->CCER |= TIM_CCER_CC3E;               
    TIM2->CCR3 = (400 * dutyCycle) / 255;                             
    TIM2->CR1 |= TIM_CR1_ARPE;                 
    TIM2->CR1 |= TIM_CR1_CEN;                 
}
void configureTimerPWM_D5(uint8_t dutyCycle) {
    TIM3->CR1 &= ~TIM_CR1_CEN;          
    TIM3->PSC = 16 - 1;                       
    TIM3->ARR = 400 - 1;                     
    
    // Configure channel 1
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;           
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);  
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;           
    TIM3->CCER |= TIM_CCER_CC1E;              
    TIM3->CCR1 = (400 * dutyCycle) / 255;                           
    TIM3->CR1 |= TIM_CR1_ARPE;                 
    TIM3->CR1 |= TIM_CR1_CEN;                 
}

void configureOnBoardLED(void){ 
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;       // enables GPIOA clocks
    GPIOA->MODER |= (1 << 10);                 // Set PA5 as output
}

#endif
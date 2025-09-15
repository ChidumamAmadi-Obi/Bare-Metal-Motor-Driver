#ifndef ADC_H
#define ADC_H

#include "stm32f4xx.h"

void ADCInit(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                                   
    GPIOA->MODER |= GPIO_MODER_MODER0;                                      
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;                                   
    ADC1->CR2 = 0;                                                          // Configure ADC Disable ADC first
    ADC1->CR1 &= ~ADC_CR1_RES;                                              
    ADC1->SMPR2 |= ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_2;  // Set sample time for channel 0 (480 cycles)
    ADC1->SQR1 &= ~ADC_SQR1_L;                                              // Set regular sequence length to 1
    ADC1->SQR3 = 0;                                                         
    ADC1->CR2 |= ADC_CR2_CONT;                                              
    ADC1->CR2 |= ADC_CR2_ADON;                                             
    for(volatile int i = 0; i < 1000; i++);                                 // Wait for ADC to stabilize
}

uint16_t ADCReadA0(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART;       
    while(!(ADC1->SR & ADC_SR_EOC));    
    return ADC1->DR;
}

#endif
#ifndef ADC_H
#define ADC_H

#include "stm32f4xx.h"
#include "config.h"

static uint16_t adcReadings[NUMREADINGS];
static uint16_t readIndex = 0;
static uint16_t total = 0;

uint16_t getAvg( uint16_t reading ) { // gets average reading out of a constant number of readings
    total = total - adcReadings[readIndex]; 
    adcReadings[readIndex] = reading;
    total = total + adcReadings[readIndex];
    readIndex++;
    if (readIndex >= NUMREADINGS) readIndex = 0;
    return (uint16_t)(total / NUMREADINGS);
}
void configureADC1GPIO(ADC1_PINS pin) { 
    GPIOA->MODER |= (0x3 << (2 * pin));  // Configure pin as analog mode 
    GPIOA->PUPDR &= ~(0x3 << (2 * pin)); // Disable pull-up/pull-down
}
void ADC1Init(ADC1_PINS pin) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    configureADC1GPIO(pin); // Configure GPIO for analog mode
    ADC1->CR2 = 0;                          // Clear
    ADC1->CR1 &= ~ADC_CR1_RES;              // 12-bit resolution
    
    if (pin < 10) { // Configure sample time (480 cycles for all channels)
        ADC1->SMPR2 |= (0x7 << (3 * pin)); 
    } else {
        ADC1->SMPR1 |= (0x7 << (3 * (pin - 10))); // Channels 10-15
    }
    ADC1->SQR1 &= ~ADC_SQR1_L;              
    ADC1->SQR3 = pin;         
    ADC1->CR2 |= ADC_CR2_CONT;              
    ADC1->CR2 |= ADC_CR2_ADON;

    delayMs(10); // Wait for ADC to stabalize 
    for (int i = 0; i < NUMREADINGS; i++) adcReadings[i] = 0; // initialize all readings to 0
}
uint16_t ADC1Read(ADC1_PINS pin) {
    if (ADC1->SQR3 != pin) { // Updates the channel if different from current
        ADC1->SQR3 = pin;
        delayMs(1);
    }
    ADC1->CR2 |= ADC_CR2_SWSTART;           
    while(!(ADC1->SR & ADC_SR_EOC));
    uint16_t rawadc = ADC1->DR; // reads ADC
    return getAvg(rawadc); // returns averaged value
}

#endif

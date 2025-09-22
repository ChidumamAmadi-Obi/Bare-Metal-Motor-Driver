#ifndef ADC_H
#define ADC_H

#include "stm32f4xx.h"
#include "config.h"

const uint8_t pinToChannel[] = {
    0, // PA0 - ADC1_IN0
    1, // PA1 - ADC1_IN1
    2, // PA2 - ADC1_IN2
    3, // PA3 - ADC1_IN3
    4, // PA4 - ADC1_IN4
    5, // PA5 - ADC1_IN5
    6, // PA6 - ADC1_IN6
    7  // PA7 - ADC1_IN7
};

static uint16_t adcReadings[NUMREADINGS];
static uint16_t readIndex = 0;
static uint16_t total = 0;

uint16_t getAvg( uint16_t reading ) {
    total = total - adcReadings[readIndex]; 
    adcReadings[readIndex] = reading;
    total = total + adcReadings[readIndex];
    readIndex++;
    if (readIndex >= NUMREADINGS) readIndex = 0;
    return (uint16_t)(total / NUMREADINGS);
}
static void configureAnalogGPIO(uint8_t pin) {
    GPIOA->MODER |= (0x3 << (2 * pin));  // Configure pin as analog mode 
    GPIOA->PUPDR &= ~(0x3 << (2 * pin)); // Disable pull-up/pull-down
}
void ADCInit(uint8_t pin) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    configureAnalogGPIO(pin); // Configure GPIO for analog mode
    ADC1->CR2 = 0;                         
    ADC1->CR1 &= ~ADC_CR1_RES;              // 12-bit resolution
    
    if (pin < 10) { // Configure sample time (480 cycles for all channels)
        ADC1->SMPR2 |= (0x7 << (3 * pin)); 
    } else {
        ADC1->SMPR1 |= (0x7 << (3 * (pin - 10))); // Channels 10-15
    }
    ADC1->SQR1 &= ~ADC_SQR1_L;              
    ADC1->SQR3 = pinToChannel[pin];         
    ADC1->CR2 |= ADC_CR2_CONT;              
    ADC1->CR2 |= ADC_CR2_ADON;            
    
    for(volatile int i = 0; i < 1000; i++); // Wait for ADC to stabilize

    for (int i = 0; i < NUMREADINGS; i++) adcReadings[i] = 0; // initialize all readings to 0

}
uint16_t ADCRead(uint8_t pin) {
    if (ADC1->SQR3 != pinToChannel[pin]) { // Updates the channel if different from current
        ADC1->SQR3 = pinToChannel[pin];
        delayMs(1);
    }
    ADC1->CR2 |= ADC_CR2_SWSTART;           
    while(!(ADC1->SR & ADC_SR_EOC));
    uint16_t rawadc = ADC1->DR;
    return getAvg(rawadc);
}


#endif

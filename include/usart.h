#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdarg.h>
#include "stm32f4xx.h"

#ifndef SystemCoreClock
#define SystemCoreClock 16000000 // 16MHz in datasheet
#endif

void usart2Init(uint32_t baud_rate) { // Initialize USART2 with specified baud rate 
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // Configures PA2 (TX) and PA3 (RX) for alternate function
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (2 << GPIO_MODER_MODER2_Pos) | (2 << GPIO_MODER_MODER3_Pos);
    
    // Sets alternate function to AF7 (USART2)
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
    
    // Configures USART2
    USART2->BRR = SystemCoreClock / baud_rate;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE;  
    USART2->CR1 |= USART_CR1_UE;               
}
void usart2SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE)); 
    USART2->DR = (c & 0xFF);
}
void usart2SendString(const char *str) {
    while (*str) {
        usart2SendChar(*str++);
    }
}
void usart2SendNumber(int32_t num) {
    char buffer[12]; 
    char *ptr = buffer;
    int32_t n = num;
    
    if (n < 0) {
        usart2SendChar('-');
        n = -n;
    }
    
    
    do { // Convert number to string (reverse order)
        *ptr++ = '0' + (n % 10);
        n /= 10;
    } while (n > 0);
    
    
    while (ptr > buffer) { // Print in correct order
        usart2SendChar(*--ptr);
    }
}
void usart2SendHex(uint32_t num) {
    usart2SendString("0x");
    
    if (num == 0) {
        usart2SendChar('0');
        return;
    }
    
    
    uint8_t started = 0; // Find the position of the first non-zero hex digit
    for (int8_t i = 7; i >= 0; i--) {
        uint8_t nibble = (num >> (i * 4)) & 0xF;
        if (nibble != 0 || started || i == 0) {
            started = 1;
            usart2SendChar(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);
        }
    }
}
void usart2NewLine(void) {
    usart2SendString("\r\n");
}
static void simplePrintf(const char *format, va_list args) {
    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': {
                    int num = va_arg(args, int);
                    usart2SendNumber(num);
                    break;
                }
                case 'x': {
                    uint32_t num = va_arg(args, uint32_t);
                    usart2SendHex(num);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    usart2SendChar(c);
                    break;
                }
                case 's': {
                    char *str = va_arg(args, char*);
                    usart2SendString(str);
                    break;
                }
                default:
                    usart2SendChar(*format);
                    break;
            }
        } else {
            usart2SendChar(*format);
        }
        format++;
    }
}
void usart2Printf(const char *format, ...) {// Format and send string
    va_list args;
    va_start(args, format);
    simplePrintf(format, args);
    va_end(args);
}

#endif 
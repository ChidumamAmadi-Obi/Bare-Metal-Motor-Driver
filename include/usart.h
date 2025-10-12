#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "config.h"

volatile char CMDBuffer[CMD_BUFFER_SIZE]; // holds incoming USART chars until the user hits ENTER (/r) or (/n)
volatile uint8_t CMDIndex = 0;
volatile uint8_t CMDReady = 0;

void usart2Init(uint32_t baud_rate) { // Initialize USART2 with specified baud rate 
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    
    // Configures PA2 (TX) and PA3 (RX) for alternate function
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (2 << GPIO_MODER_MODER2_Pos) | (2 << GPIO_MODER_MODER3_Pos);
    
    //Select USART2 as the Alternate Function
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);
    
    // Configures USART2
    USART2->BRR = SystemCoreClock / baud_rate;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;   
    USART2->CR1 |= USART_CR1_UE;
    
    NVIC_SetPriority(USART2_IRQn, 0);
    NVIC_EnableIRQ(USART2_IRQn);  // enables interrupt    
}

void usart2SendChar(char c) { // sends character via tx
    while (!(USART2->SR & USART_SR_TXE)); 
    USART2->DR = (c & 0xFF);
}
void usart2SendString(const char *str) {
    while (*str) { // when not at the end of a string:   while (*str != '\0')
        usart2SendChar(*str++); // builds string until '\0'
    }
}
void usart2SendNumber(int32_t num) {
    char buffer[12]; // 12-bit, enough for -2,147,483,648
    char *ptr = buffer;
    int32_t n = num; // copies input number
    
    if (n < 0) { // for negative numbers
        usart2SendChar('-');
        n = -n;
    }

    do { // digit extraction, but its reversed , e.g. 123 -> ['3','2','1']
        *ptr++ = '0' + (n % 10);
        n /= 10;
    } while (n > 0);

    // ^^ in english  Take the last digit of the number                              (n % 10) equals the remainder when n/10 
    //                convert it to a character                                      '0' + a number turns that number into a char
    //                store that character where the pointer is currently looking    *ptr = '0' + (n % 10)
    //                then move the pointer to the next spot in memory.              *ptr++ = '0' + (n % 10)
    //                repeat until n is zero 
    
    while (ptr > buffer) usart2SendChar(*--ptr); // Prints in correct order

    // ^^ in english: when the pointer is not at the strting position                       while ( ptr > buffer )
    //                move the pointer backward one position, happens before dereference    --ptr
    //                then send the character it's now pointing at.                         usart2SendChar(*--ptr);
}
void usart2NewLine(void) {
    usart2SendString("\r\n");
}
void simplePrintf(const char *format, va_list args) {
    while (*format) { // loop until you reach the end of the format string (null terminator)
        if (*format == '%') { // if char is '%'
            format++;
            switch (*format) {
                case 'd': 
                    int num = va_arg(args, int); // looks at data type that comes next (an int), reads the value of that data , moves pointer to the next arg, returns the int
                    usart2SendNumber(num);
                    break;
                case 'c': 
                    char c = (char)va_arg(args, int);
                    usart2SendChar(c);
                    break;                
                case 's': 
                    char *str = va_arg(args, char*); // looks for data type that comes next ( a pointer to the start of a string), reads the value, moves pointer to the next arg, returns the address to the start of the string
                    usart2SendString(str);
                    break;                
                default:
                    usart2SendChar(*format);
                    break;
            }
        } else usart2SendChar(*format); // just send the character as is
        format++;
    }
}
void usart2Printf(const char *format, ...) {// Format and send string
    va_list args; // create va_list object for simple print
    va_start(args, format); // start reading variable arguments that come AFTER the format parameter
    simplePrintf(format, args); // Pass both the format string and the prepared argument list into simple printf
    va_end(args);
}

void USART2_IRQHandler() { // gets called automatically when character is received
    if (USART2 -> SR & USART_SR_RXNE) {
        char c = USART2->DR; // reads char
        usart2SendChar(c); // Echo the character back to terminal
        if (c == ASCII_BACKSPACE) { // Backspace handling
            if (CMDIndex > 0) {
                CMDIndex--;
            }
        } else if (c == '\r') {  
            CMDBuffer[CMDIndex] = '\0';  // null-terminate
            CMDIndex = 0;
            CMDReady = 1; 
            usart2NewLine(); 
        } else if ( c == '\n') {
            // ignore 
        } else {
            if (CMDIndex < CMD_BUFFER_SIZE - 1) {
                CMDBuffer[CMDIndex++] = c;
            }
        }
    }
}

#endif
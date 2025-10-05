#ifndef CONFIG
#define CONFIG

#include "stm32f4xx.h"

#define CMD_BUFFER_SIZE 30
#define SystemCoreClock 16000000 // 16MHz in datasheet

#define NUMREADINGS 15 // for averaging adc vals

#define ASCII_BACKSPACE 0x7F // used in puTTy's terminal
#define ASCII_ESC 0x1B
#define ASCII_OPENING_BRACKET 0x5B
#define ASCII_UP 0x41
#define ASCII_DOWN 0x42
#define ASCII_RIGHT 0x43
#define ASCII_LEFT 0x44

#define MIN_DEADBAND_THRESHOLD 1048 // adc values for deadband
#define MAX_DEADBAND_THRESHOLD 3048
#define MAX_LEFT 32 // top left spinning speed at adc value 35
#define MAX_RIGHT 4096 // top right spinning speed at adc value 4096

// Enums used here and passed to various functions
// to create named constants and make the code type-safe.
typedef enum { // ahb1 peripheral clocks
    CLOCK_GPIOA = RCC_AHB1ENR_GPIOAEN,
    CLOCK_GPIOB = RCC_AHB1ENR_GPIOBEN,
    CLOCK_GPIOC = RCC_AHB1ENR_GPIOCEN,
    CLOCK_GPIOD = RCC_AHB1ENR_GPIODEN,
    CLOCK_GPIOE = RCC_AHB1ENR_GPIOEEN,
    CLOCK_ALL_GPIO = RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | 
                     RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | 
                     RCC_AHB1ENR_GPIOEEN
} AHB1_Periphs_t;
typedef enum { // apb1 peripheral clocks
    CLOCK_TIM2 = RCC_APB1ENR_TIM2EN,
    CLOCK_TIM3 = RCC_APB1ENR_TIM3EN,
    CLOCK_TIM4 = RCC_APB1ENR_TIM4EN,
    CLOCK_TIM5 = RCC_APB1ENR_TIM5EN
} APB1_Periphs_t;
typedef enum { // apb2 peripheral clocks
    CLOCK_USART1 = RCC_APB2ENR_USART1EN,
    CLOCK_TIM1 = RCC_APB2ENR_TIM1EN
} APB2_Periphs_t;
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_ALTERNATE = 2,
    GPIO_MODE_ANALOG = 3
} GPIOMode_t;
typedef enum {
    GPIO_OUTPUT_PP = 0, // push-pull
    GPIO_OUTPUT_OD = 1 //
} GPIOOutputType_t;
typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM = 1,
    GPIO_SPEED_HIGH = 2,
    GPIO_SPEED_VERY_HIGH = 3
} GPIOSpeed_t;
typedef enum {
    PA0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
} ADC1_PINS;
typedef enum { // written to port c to control LED visualizer
    LEDS_OFF = 0x00,
    LED_LEFT1 = 0x01,
    LED_LEFT2 = 0x03,
    LED_LEFT3 = 0x07,
    LED_LEFT4 = 0x0F,
    LED_RIGHT1 = 0x10,
    LED_RIGHT2 = 0x30,
    LED_RIGHT3 = 0x70,
    LED_RIGHT4 = 0xF0,
} LED_BAR_GRAPH_MODES;

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
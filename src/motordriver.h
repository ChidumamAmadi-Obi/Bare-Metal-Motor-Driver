#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "stm32f4xx.h"

#include <string.h>   // strtok, strcmp
#include <stdlib.h>   // atoi
#include <stddef.h>   // NULL
#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "usart.h"
#include "adc.h"
#include "gpio.h"

static uint8_t prevSpeed = 0;
static uint8_t speed = 0;
static char direction = 'N';

static bool manualMode = 1;
static bool power = 1;
static bool monitorMode = 0;

// Initlalize system
void systemInit(){
    SysTickInit();
    enableClocks(CLOCK_GPIOA | CLOCK_GPIOB | CLOCK_GPIOC ,CLOCK_TIM2 | CLOCK_TIM3, 0);
    
    usart2Init(115200);
    usart2Printf("==================================================\r\n");
    usart2Printf("  Clocks enabled\r\n");
    usart2Printf("  USART2 Initialized, 115200 baud\r\n");
    usart2Printf("  System clock: %d Hz\r\n", SystemCoreClock);

    ADC1Init(PA0);
    usart2Printf("  ADC Pin PA0 Initialized\r\n");

    configGPIO_PWM();
    configGPIO_Port(GPIOC, GPIO_MODE_OUTPUT, GPIO_OUTPUT_PP, GPIO_SPEED_HIGH); // configures PC0-PC7 as outputs, push-pull, high speed
    usart2Printf("  GPIO Pins configured\r\n");

    configTimerPWM_D6(0); configTimerPWM_D5(0); // D6 and D5 with TIM2 and TIM3 - 10Hz, 0% duty cycle
    
    usart2Printf("  Starting D6 and D5 on 0 percent duty cycle...\r");
    usart2Printf("--------------------------------------------------\r\n");
    usart2Printf("  Manual Mode enabled, control via potentiometer\r\n");
    usart2Printf("  Type 'HELP' for CLI information\r\n");
    usart2Printf("==================================================\r\n\n");
    configureOnBoardLED();
}

// Motor control 
inline uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void handleMotorConrtol(){
    if (power) {
        if (direction == 'L') {
            configTimerPWM_D5(speed);
            configTimerPWM_D6(0);
        } else if (direction == 'R') {
            configTimerPWM_D5(0);
            configTimerPWM_D6(speed);
        }
    } else {
        speed = 0;
        direction = 'N';
        configTimerPWM_D5(0);
        configTimerPWM_D6(0); 
    }    
}
void manualInputReceiver(uint16_t adcValue){
    if ( (adcValue > MIN_DEADBAND_THRESHOLD && adcValue < MAX_DEADBAND_THRESHOLD) || !power){ // if adc in deadband
        speed = 0;
    } else {
        if ( adcValue < MIN_DEADBAND_THRESHOLD ) {
            speed = map(adcValue, MAX_LEFT, MIN_DEADBAND_THRESHOLD, 100, 0);
            direction = 'L';
        }
        if ( adcValue > MAX_DEADBAND_THRESHOLD ) {
            speed = map(adcValue, MAX_RIGHT, MAX_DEADBAND_THRESHOLD, 100, 0);
            direction = 'R';
        }
    }
}
void handleLEDVisualizer(){
    static uint8_t LEDBarGraphL;
    static uint8_t LEDBarGraphR;

    if (direction =='L')        {
        LEDBarGraphL = map(speed,0,100,1,4); 
        LEDBarGraphR = 0;
    } else if (direction == 'R') {
        LEDBarGraphR = map(speed,0,100,1,4); 
        LEDBarGraphL = 0;
    }
    
    if (!power || speed == 0) writeToPort(GPIOC, LEDS_OFF);
    else if ( power ) {
        if (LEDBarGraphR == 0) {
            switch (LEDBarGraphL) { // toggles left LED bar graph
                case 1:  writeToPort(GPIOC, LED_LEFT1); break;
                case 2:  writeToPort(GPIOC, LED_LEFT2); break;
                case 3:  writeToPort(GPIOC, LED_LEFT3); break;
                case 4:  writeToPort(GPIOC, LED_LEFT4); break;
                default: writeToPort(GPIOC, LEDS_OFF); break; 
            }
        } else if (LEDBarGraphL == 0) {
            switch (LEDBarGraphR) { // toggles right LED bar graph
                case 1:   writeToPort(GPIOC, LED_RIGHT1); break;
                case 2:   writeToPort(GPIOC, LED_RIGHT2); break;
                case 3:   writeToPort(GPIOC, LED_RIGHT3); break;
                case 4:   writeToPort(GPIOC, LED_RIGHT4); break;
                default:  writeToPort(GPIOC, LEDS_OFF); break; 
            }
        }
    }
}

// CLI functions
void monitorADC(uint8_t adcValue) {
    if ( speed != prevSpeed ) {
        usart2Printf("ADC Value: %d Speed: %d%%\r\n", adcValue, speed);
        prevSpeed = speed;
    }
}
void printStatus(uint16_t adcValue){    
    usart2Printf("   Power       "); usart2Printf((power) ? "ON\r\n" : "OFF\r\n");
    usart2Printf("            Control     "); usart2Printf((manualMode) ? "MANUAL\r\n" : "CLI\r\n");
    usart2Printf("            ADC Value   %d/4096\r\n", adcValue);
    usart2Printf("            Speed       %d%%\r\n", speed); 
    usart2Printf("            Direction   %c", direction);

}
void CLIcommandParser(uint8_t adcValue, char *input) {
    char *cmd = strtok(input, " "); // strtok parses incoming data
    char *arg1 = strtok(NULL," ");
    char *arg2 = strtok(NULL," ");
    char *arg3 = strtok(NULL," ");
    char *arg4 = strtok(NULL," ");
    if (*input == '\0') usart2Printf("\nSTM32 -> ERROR: Empty command\r\n\n");
    if (strcmp(cmd, "MANUAL") == 0 && arg1) {
        if (strcmp(arg1, "OFF") == 0) {
            manualMode = 0;
            speed = 0;
            usart2Printf("\nSTM32 -> Manual control disabled. Motor now controlled via CLI.\r\n");
        } else if ( strcmp(arg1, "ON") == 0) {
            manualMode = 1;
            usart2Printf("\nSTM32 -> Manual control enabled. Motor now controlled via potentiometer.\r\n\n");
        } else {
            usart2Printf("\nSTM32 -> ERROR: Invalid MANUAL argument. Use ON or OFF.\r\n\n");
        }

    } else if (strcmp(cmd, "STATUS") == 0) { // print current status of system variables
        usart2Printf("\nSTM32 -> ");
        printStatus(adcValue);
        usart2Printf("\r\n\n");

    } else if (strcmp(cmd, "SET") == 0 && arg1 && strcmp(arg1, "SPEED") == 0 && arg2 && arg3 && strcmp(arg3, "DIR") == 0 && arg4) {
        if (!manualMode) {
            speed = atoi(arg2);
            usart2Printf("\nSTM32 -> Speed set to %d, Direction set to ", speed);
            if (speed <= 255 && speed >= 0) {
                if (strcmp(arg4, "L") == 0) {
                    direction = 'L';
                    usart2Printf("Left\r\n\n");

                } else if (strcmp(arg4, "R") == 0) {
                    direction = 'R';
                    usart2Printf("Right\r\n\n");

                } else {
                    usart2Printf("\nSTM32 -> ERROR: Invalid motor direction. Use L (Left), R (Right), or N (OFF).\r\n\n");
                }
            } else {
                usart2Printf("\nSTM32 -> ERROR: Speed set out of bounds, (0-100)\r\n\n");
            }
        } else {
            usart2Printf("\nSTM32 -> ERROR: Turn manual mode off before CLI motor control!\r\n\n");
        }

    } else if (strcmp(cmd, "POWER") == 0) {
        if (strcmp(arg1, "OFF") == 0) {
            usart2Printf("\nSTM32 -> Motors powered off.\r\n\n");
            power = 0;
        } else if (strcmp(arg1, "ON") == 0) {
            usart2Printf("\nSTM32 -> Motors powered on.\r\n\n");
            power = 1;
        } else {
            usart2Printf("\nSTM32 -> ERROR: Invalid power argument, use ON or OFF\r\n\n");
        }
    } else if (strcmp(cmd, "M-") == 0) {
        monitorMode = !monitorMode;
        usart2Printf("\nSTM32 -> ADC Monitoring "); 
        usart2Printf((monitorMode) ? "enabled\r\n" : "disabled\r\n");
    } else if (strcmp(cmd, "HELP") == 0) {
        usart2Printf("\nSTM32 -> AVAILIBLE COMMANDS:\r\n");
        usart2Printf("            HELP                Show this message\r\n");
        usart2Printf("            STATUS              Show current motor speed, direction, and potentiometer adc value\r\n");
        usart2Printf("            SET SPEED x DIR x   Set motor speed and direction, (0-100), L (Left), R (Right), or N (OFF) \r\n");
        usart2Printf("            POWER x             Powers motors on or off, ON/OFF \r\n");
        usart2Printf("            M-                  Enables/Disables constant monitoring messages \r\n");
        usart2Printf("            MANUAL x            Enables/Disables manual motor control via potentiometer, Use ON, or OFF\r\n\n");
    } else {
        usart2Printf("\nSTM32 -> ERROR: Invalid command. Type 'HELP' for more information.\r\n\n");
    }
}


#endif

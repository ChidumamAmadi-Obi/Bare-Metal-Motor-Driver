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

static uint8_t motorSpeedL;
static uint8_t motorSpeedR;
static uint8_t prevSpeed = 0;
static uint8_t speed;
static char direction;

bool manualMode = 1;
bool power = 1;
bool monitorMode = 0;

// Initlalize system
void systemInit(){
    SysTickInit();
    usart2Init(115200);
    usart2Printf("==================================================\r\n");
    usart2Printf("  USART2 Initialized\r\n");
    usart2Printf("  System clock: %d Hz\r\n", SystemCoreClock);

    ADCInit(0);
    usart2Printf("  ADC Pin PA0 Initialized\r\n");

    enableClocks();
    usart2Printf("  Clocks TIM2 and TIM3 configured\r\n");
    usart2Printf("  Clocks for GPIO ports A, B and C configured\r\n");

    configGPIO();
    usart2Printf("  GPIO Pins D5 and D6 configured\r\n");

    configTimerPWM_D6(0); // D6 and D5 with TIM2 and TIM3 - 10Hz, 0% duty cycle
    configTimerPWM_D5(0);
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
void handleMotorConrtol(uint8_t motorSpeedL, uint8_t motorSpeedR){
    if (power) {
        configTimerPWM_D5(motorSpeedL);
        configTimerPWM_D6(motorSpeedR); 
    } else {
        configTimerPWM_D5(0);
        configTimerPWM_D6(0); 
    }
    
}
void manualInputReceiver(uint16_t adcValue){
    if ( (adcValue > MIN_DEADBAND_THRESHOLD && adcValue < MAX_DEADBAND_THRESHOLD) || !power){ // if adc in deadband
        motorSpeedR = 0;
        motorSpeedL = 0;
    } else {
        if ( adcValue < MIN_DEADBAND_THRESHOLD ) {
            motorSpeedL = map(adcValue, 0, MIN_DEADBAND_THRESHOLD, 100, 0);
            motorSpeedR = 0;
        }
        if ( adcValue > MAX_DEADBAND_THRESHOLD ) {
            motorSpeedR = map(adcValue, 4096, MAX_DEADBAND_THRESHOLD, 100, 0);
            motorSpeedL = 0;
        }
    }
    if ( !power || (motorSpeedL == 0 && motorSpeedR == 0)) {
        speed = 0;
        direction = 'N';
    } else if ( motorSpeedL > 0) {
        speed = motorSpeedL;
        direction = 'L';
    } else if ( motorSpeedR > 0) {
        speed = motorSpeedR; 
        direction = 'R';
    }
}
void handleLEDVisualizer(){
    uint8_t LEDBarGraphL = map(motorSpeedL,0,100,1,4);
    uint8_t LEDBarGraphR = map(motorSpeedR,0,100,1,4);
    
    if ( motorSpeedL == 0 ) LEDBarGraphL = 0;
    if ( motorSpeedR == 0 ) LEDBarGraphR = 0;
    if (!power) GPIOC->ODR = 0x00;
    else if ( power ) {
        if (LEDBarGraphR == 0) {
            switch (LEDBarGraphL) { // toggles left LED bar graph
                case 1:
                    GPIOC->ODR = 0x01;
                    break;
                case 2:
                    GPIOC->ODR = 0x03; 
                    break;
                case 3:
                    GPIOC->ODR = 0x07; 
                    break;
                case 4:
                    GPIOC->ODR = 0x0F; 
                    break;

                default: 
                    GPIOC->ODR = 0x00; // all LEDs off
                    break;
            }
        } else if (LEDBarGraphL == 0) {
            switch (LEDBarGraphR) { // toggles right LED bar graph
                case 1:
                    GPIOC->ODR = 0x10; 
                    break;
                case 2:
                    GPIOC->ODR = 0x30; 
                    break;
                case 3:
                    GPIOC->ODR = 0x70; 
                    break;
                case 4:
                    GPIOC->ODR = 0xF0; 
                    break;

                default:
                    GPIOC->ODR = 0x00; // all LEDs off
                    break;
            }
        }
    }
}

// CLI functions
void monitorADC(uint8_t adcValue) {
    if ( speed != prevSpeed ) {
        usart2Printf("\nADC Value: %d Speed: %d%%\r\n", adcValue, speed);
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
void CLIcommandParser(uint16_t adcValue, char *input) {
    char *cmd = strtok(input, " ");
    char *arg1 = strtok(NULL," ");
    char *arg2 = strtok(NULL," ");
    char *arg3 = strtok(NULL," ");
    char *arg4 = strtok(NULL," ");
    if (*input == '\0') usart2Printf("\nSTM32 -> ERROR: Empty command\r\n\n");
    if (strcmp(cmd, "MANUAL") == 0 && arg1) {
        if (strcmp(arg1, "OFF") == 0) {
            manualMode = 0;
            usart2Printf("\nSTM32 -> Manual control disabled. Motor now controlled via CLI.\r\n\n");
        } else if ( strcmp(arg1, "ON") == 0) {
            manualMode = 1;
            usart2Printf("\nSTM32 -> Manual control enabled. Motor now controlled via potentiometer.\r\n\n");
        } else {
            usart2Printf("\nSTM32 -> ERROR: Invalid MANUAL argument. Use ON or OFF.\r\n\n");
        }

    } else if (strcmp(cmd, "STATUS") == 0) {
        usart2Printf("\nSTM32 -> ");
        printStatus(adcValue);
        usart2Printf("\r\n\n");

    } else if (strcmp(cmd, "SET") == 0 && arg1 && strcmp(arg1, "SPEED") == 0 && arg2 && arg3 && strcmp(arg3, "DIR") == 0 && arg4) {
        if (!manualMode) {
            speed = atoi(arg2);
            usart2Printf("\nSTM32 -> Speed set to %d, Direction set to ", speed);
            if (speed <= 255 && speed >= 0) {
                if (strcmp(arg4, "L") == 0) {
                    motorSpeedL = 0;
                    motorSpeedR = speed;
                    usart2Printf("Left\r\n\n");

                } else if (strcmp(arg4, "R") == 0) {
                    motorSpeedL = speed;
                    motorSpeedR = 0;
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
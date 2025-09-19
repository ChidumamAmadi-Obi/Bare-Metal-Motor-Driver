# Bare-Metal STM32 Motor Driver, PWM + ADC Control without HAL
![In Progress Badge](https://img.shields.io/badge/status-in%20progress-orange)  

A **Bare-metal** motor control system implemented on **STM32 Nucleo-F401RE**, featuring **register-level** peripheral control, **analog sensor processing**, **hardware PWM generation**, and a **CLI interface** - all without HAL, CubeMX, or Arduino frameworks.

## Control Behaviour 
* Turn the potentiometer **left** - motor spins left / left LED brightens.
* Turn the potentiometer **right** - motor spins right / right LED brightens.
* Center potentiometer - motor stops / LEDs off.
* CLI commands - full control and monitoring via serial interface.
* Visualize motor speed with LED bar graph.

## Demos 
### Direction Control, PWM & LED Visualizer
![IMG_2973-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/e9ab0777-c68f-48c7-974d-36f4139dba0c)

### CLI Demonstration
![2025-09-19-193643-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/b5bf2980-ab52-4334-b76e-ebab79aabcd2)

### Preformance Metrics
* **ADC Resolition**: 12-bit (0-495 values)
* **PWM Frequency**: 100hz with 8 bit duty cycle resulution for cmooth control
* **Sampling Rate**: 480-cycle sampling

### Components Required
* STM32 Nucleo-F401RE
* L9110H motor driver IC
* 1 kΩ potentiometer
* Resistors (current limiting for LEDs)
* 8 LEDs (for speed visualizer)
* DC Motor (or 2 LEDs for final test)
* Breadboard and jumper wires

### Roadmap
**Phase 1: Basic**
 - [x]  Blink LED using GPIO registers
 - [x]  Print debug info to serial (USART2)
 - [x]  Read potentiometer value via ADC1

**Phase 2: PWM & Direction Control**
 - [x] Configure TIM2 for PWM output
 - [x] Map pot value to PWM duty cycle
 - [x] Drive LEDs with brightness proportional to pot position

**Phase 3: LED Visualizer, CLI & Motor Integration**
 - [x] Implement **LED bar graph visualizer** (8–10 LEDs, showing direction + speed)
 - [x] Impliment CLI control
 - [ ] Replace LEDs with DC motor via L9110H  

### Getting Started

#### Prerequisites
* Ensure PlatformIO is installed. (Either via VSCode + PlatformIO plugin, or PlatformIO Core (CLI)).
* Ensure toolchain and board support for STM32F401RE (CMSIS) is installed. PlatformIO should pull that.
* Development Enviroment *platformio.ini*
```
# PlatformIO configuration (platformio.ini)
[env:nucleo_f401re]
platform = ststm32
board = nucleo_f401re
framework = cmsis
upload_protocol = stlink
monitor_speed = 115200
```

#### Setup
1) Open in VSCode with PlatformIO
2) Connect components as per wiring diagram
```
_________________________
- PWM Pins
L9110 Pin 6 = D5 or PB14
L9110 Pin 7 = D6 or PB15
_________________________
- Analog Input Pin
ADC = PA0
_________________________
- LED Visualizer Pins
LED_L0 = PC0 left side
LED_L1 = PC1
LED_L2 = PC2
LED_L3 = PC3
LED_R0 = PC4 right side
LED_R1 = PC5
LED_R2 = PC6
LED_R3 = PC7
```
3) Clone, build and Flash
```
git clone https://github.com/ChidumamAmadi-Obi/Bare-Metal-Motor-Driver.git
cd Bare-Metal-Motor-Driver
pio run # Build project
pio run --target upload # Upload to board
pio device monitor # Monitor serial output
```
5) Send and receive data via USART with PuTTY
<img width="1919" height="544" alt="image" src="https://github.com/user-attachments/assets/b99d3a66-4c1f-478a-8ddf-148e24acd968" />
 ^ example output of CLI

### References
* [STM32F401RE Reference Manual (RM0368)](https://www.st.com/resource/en/reference_manual/rm0368-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* [CMSIS for STM32F4 (ST CMSIS Pack)](https://github.com/STMicroelectronics/cmsis-device-f4)
* [L9110H Motor Driver datasheet](https://cdn-shop.adafruit.com/product-files/4489/4489_datasheet-l9110.pdf)


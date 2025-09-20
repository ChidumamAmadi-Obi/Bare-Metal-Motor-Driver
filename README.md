# Bare-Metal STM32 Motor Driver, PWM + ADC Control without HAL
![In Progress Badge](https://img.shields.io/badge/status-in%20progress-orange)  

A **Bare-metal** motor control system implemented on **STM32 Nucleo-F401RE**, featuring **register-level** peripheral control, **analog sensor processing**, **hardware PWM generation**, and a **CLI interface** - all without HAL, CubeMX, or Arduino frameworks.

## Demos 
### Direction Control, PWM & LED Visualizer
![IMG_2973-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/e9ab0777-c68f-48c7-974d-36f4139dba0c)

### CLI Demonstration
![2025-09-19-193643-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/b5bf2980-ab52-4334-b76e-ebab79aabcd2)

## Features
* **Bare-Metal Efficiency:** Direct register access for optimal preformance and control.
* **Dual Control Modes:**
   * **Manual Mode:** Motor speed/direction controlled by a potentiometer.
   * **CLI Mode:** Full control via a serial terminal (e.g. 'SET SPEED 200 DIR L'). 
* **Real-Time Visual Feedback:** 8-LED bar graph visually displays motor speed and direction.
* **Robust CLI Parser:** Handles commands like 'STATUS', 'HELP', 'MANUAL ON/OFF', and custom speed/direction settings.
* **Professional Software Design:** Modular drivers for USART, ADC, GPIO, and Timers.

## Control Behaviour 
* Turn the potentiometer **left** - motor spins left / left LED brightens.
* Turn the potentiometer **right** - motor spins right / right LED brightens.
* Center potentiometer - motor stops / LEDs off.
* CLI commands - full control and monitoring via serial interface.
* Visualize motor speed with LED bar graph.

## Getting Started
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
debug_tool = stlink

build_flags = 
    -D STM32F401xE
    -I include

monitor_speed = 115200 ; Serial monitor configuration
```

### Hardware Setup
* STM32 Nucleo-F401RE
* L9110H motor driver IC
* 1 kΩ potentiometer
* Resistors (current limiting for LEDs)
* 8 LEDs (for speed visualizer)
* DC Motor (or 2 LEDs for final test)
* Breadboard and jumper wires

## Wiring Diagram
<img width="787" height="594" alt="image" src="https://github.com/user-attachments/assets/9ac8df23-5357-4253-891a-04f386074f72" />

| STM32 Pin | Peripheral | Connects To |
| :--- | :--- | :--- |
| PA0 | ADC Input | Potentiometer Output |
| PA2, PA3 | USART2 (TX, RX) | USB-to-Serial Converter |
| PB4 (D5) | TIM3_CH1 | L9110H B-IA |
| PB10 (D6) | TIM2_CH3 | L9110H A-IA |
| PC0-PC7 | GPIO Output | LED Anodes (through resistors) |

#### Setup
1) Open in VSCode with PlatformIO
2) Clone, build and Flash
```
git clone https://github.com/ChidumamAmadi-Obi/Bare-Metal-Motor-Driver.git
cd Bare-Metal-Motor-Driver
pio run # Build project
pio run --target upload # Upload to board
pio device monitor # Monitor serial output
```
3) Connect to CLI by opening a serial monitor (PlatformIO, PuTTY, Tera Term, etc).
* Set baud rate to **115200**
* Type 'HELP to see all availible commands.

<img width="1919" height="544" alt="image" src="https://github.com/user-attachments/assets/b99d3a66-4c1f-478a-8ddf-148e24acd968" />
 ^ example output of CLI

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

### References
* [STM32F401RE Reference Manual (RM0368)](https://www.st.com/resource/en/reference_manual/rm0368-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* [CMSIS for STM32F4 (ST CMSIS Pack)](https://github.com/STMicroelectronics/cmsis-device-f4)
* [L9110H Motor Driver datasheet](https://cdn-shop.adafruit.com/product-files/4489/4489_datasheet-l9110.pdf)


# Bare-Metal STM32 Motor Driver, PWM + ADC Control without HAL
This project demonstrates a **bare-metal motor driver** using the **STM32 Nucleo-F401RE**, written in C with **PlatformIO and CMSIS** (no ArduinoIDE, no HAL, no CubeMX).
The goal is to drive a DC motor (or LEDs for testing) with an **L9110H motor driver IC**, controlled by a potentiometer.

* Turn the potentiometer **left** - motor spins left / left LED brightens.
* Turn the potentiometer **right** - motor spins right / right LED brightens.
* Keep the potentiometer **centered** - motor stops / LEDs off.
* Visualize motor speed with LED bar graph.

## Demo of Phase 1: Blinking an LED While Reading and printing the ADC value via USART2

![Phase1_BareMetalDriver(1)](https://github.com/user-attachments/assets/f69e2aad-3694-48d1-92f4-05018f8f1235)

### Hardware
* STM32 Nucleo-F401RE
* L9110H motor driver IC
* 1 kΩ potentiometer
* Resistors (current limiting for LEDs)
* 8 LEDs (for speed visualizer)
* DC Motor (or 2 LEDs for final test)

### Roadmap
**Phase 1: Basic**
 - [x]  Blink LED using GPIO registers
 - [x]  Print debug info to serial (USART2)
 - [x]  Read potentiometer value via ADC1

**Phase 2: PWM & Direction Control**
 - [x] Configure TIM2 for PWM output
 - [x] Map pot value to PWM duty cycle
 - [x] Drive LEDs with brightness proportional to pot position

**Phase 3: LED Visualizer & Motor Integration**
 - [ ] Implement **LED bar graph visualizer** (8–10 LEDs, showing direction + speed)  
 - [ ] Replace LEDs with DC motor via L9110H  
 - [ ] Map pot input to motor direction (left/right) and speed (PWM duty cycle)  

### Getting Started

#### Prerequisites
* Ensure PlatformIO is installed. (Either via VSCode + PlatformIO plugin, or PlatformIO Core (CLI)).
* Ensure toolchain and board support for STM32F401RE (CMSIS) is installed. PlatformIO should pull that.

#### Setup
1) Clone repo
```
git clone https://github.com/ChidumamAmadi-Obi/Bare-Metal-Motor-Driver.git
cd Bare-Metal-Motor-Driver
```
2) Open in VSCode with Platformio
3) Build and Upload with PlatformIO
```
pio run
pio upload
```

### References
* [STM32F401RE Reference Manual (RM0368)](https://www.st.com/resource/en/reference_manual/rm0368-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* [CMSIS for STM32F4 (ST CMSIS Pack)](https://github.com/STMicroelectronics/cmsis-device-f4)
* [L9110H Motor Driver datasheet](https://cdn-shop.adafruit.com/product-files/4489/4489_datasheet-l9110.pdf)


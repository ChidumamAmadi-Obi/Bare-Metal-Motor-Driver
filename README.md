# Bare-Metal STM32 Motor Driver, PWM + ADC Control without HAL
This project demonstrates a **bare-metal motor driver** using the **STM32 Nucleo-F401RE**, written in C with **PlatformIO and CMSIS** (no ArduinoIDE, no HAL, no CubeMX).
The goal is to drive a DC motor (or LEDs for testing) with an **L9110H motor driver IC**, controlled by a potentiometer.

* Turn the potentiometer **left** - motor spins left / left LED brightens.
* Turn the potentiometer **right** - motor spins right / right LED brightens.
* Keep the potentiometer **centered** - motor stops / LEDs off.

### Hardware
* STM32 Nucleo-F401RE
* L9110H motor driver IC
* 1 kΩ potentiometer
* LEDs (or DC motor for final test)
* Resistors (current limiting for LEDs)

### Roadmap
Phase 1: Basic
 - [x]  Blink LED using GPIO registers
 - [x]  Print debug info to serial (USART2)
 - [ ]  Read potentiometer value via ADC1

Phase 2: Direction Control
 - [ ] Turn on left LED when pot is tured left
 - [ ] Turn on right LED when pot is tured right
 - [ ] Turn both LEDs off when pot ~ mid

Phase 3: PWM Control
 - [ ] Configure TIM2 for PWM output
 - [ ] Map pot value to PWM duty cycle
 - [ ] Crive LEDs with brightness proportional to pot position
 - [ ] Replace LEDs with DC motor via L9110H

### Getting Started

1) Clone repo
2) Build and Upload with PlatformIO
```
pio run
pio upload
```

### References
* [STM32F401RE Reference Manual (RM0368)](https://www.st.com/resource/en/reference_manual/rm0368-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* ![CMSIS for STM32F4 (ST CMSIS Pack)](https://github.com/STMicroelectronics/cmsis-device-f4)
* ![L9110H Motor Driver datasheet](https://cdn-shop.adafruit.com/product-files/4489/4489_datasheet-l9110.pdf)


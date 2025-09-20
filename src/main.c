#include "motordriver.h"

int main() {
    systemInit();
    while(1) {
        usart2IRQHandler();  // processes incoming characters from cli
        uint16_t adcVal = ADCRead(0); 
        if (CMDReady) {
            commandParser(adcVal, (char*)CMDBuffer);
            CMDReady = 0;
            }

        if (manualMode) motorControl(adcVal);
        if (power) handleMotorConrtol(motorSpeedL, motorSpeedR);
        handleLEDVisualizer();
    }
    return 0;
}
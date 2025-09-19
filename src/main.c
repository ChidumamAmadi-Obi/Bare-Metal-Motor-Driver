#include "motordriver.h"

int main() {
    systemInit();
    while(1) {
        usart2IRQHandler();  // processes incoming characters from cli
        uint16_t adcVal = ADCReadA0(); 
        if (CMDReady) {
            commandParser(adcVal, (char*)CMDBuffer);
            CMDReady = 0;
            }

        if (manualMode) motorControl(adcVal);
        handleLEDVisualizer();
        handleMotorConrtol(motorSpeedL, motorSpeedR);
    }
    return 0;
}
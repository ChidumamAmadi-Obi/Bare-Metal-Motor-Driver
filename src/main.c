#include "motordriver.h"

int main() {
    systemInit();
    while(1) {
        uint16_t adcVal = ADCRead(0); 
        usart2IRQHandler();
        
        if (CMDReady) {
            CLIcommandParser(adcVal, (char*)CMDBuffer);
            CMDReady = 0;
            }

        if (manualMode) manualInput(adcVal);
        if (power) handleMotorConrtol(motorSpeedL, motorSpeedR); 
        handleLEDVisualizer();
    }
    return 0;
}
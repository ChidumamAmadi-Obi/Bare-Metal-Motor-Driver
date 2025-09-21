#include "motordriver.h"

int main(void) {
    systemInit();
    while(1) {
        uint16_t adcVal = ADCRead(0); 
        if (CMDReady) {
            CLIcommandParser(adcVal, (char*)CMDBuffer);
            CMDReady = 0;
            }

        if (manualMode) manualInputReceiver(adcVal);
        if (monitorMode) monitorADC(adcVal);
        handleMotorConrtol(motorSpeedL, motorSpeedR); 
        handleLEDVisualizer();
    }
    return 0;
}
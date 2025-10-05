#include "motordriver.h"

int main(void) {
    systemInit();
    while(1) {
        uint8_t adcVal = ADC1Read(PA0);
        if (CMDReady) {
            CLIcommandParser(adcVal, (char*)CMDBuffer);
            CMDReady = 0;
            }
        if (manualMode)  manualInputReceiver(adcVal);
        if (monitorMode) monitorADC(adcVal);
        handleMotorConrtol(); 
        handleLEDVisualizer();
    }
    return 0;
}
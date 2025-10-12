#include "motordriver.h"

int main(void) {
    systemInit();
    uint8_t adcVal;
    while(1) {
        adcVal = ADC1Read(PA0);
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
#include "motordriver.h"

int main() {
    systemInit();
    while(1) {
        // GPIOA->ODR ^= (1 << 5);            // Toggle onboard LED
        motorControl(ADCReadA0());
    }
    return 0;
}
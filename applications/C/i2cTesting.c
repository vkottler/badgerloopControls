#include "../include/i2cTesting.h"

char message[100];

void I2Cinitializers(void) {
    
    // hardware
    __builtin_disable_interrupts();
    
    initUART();
    I2Cinit(1, 100, true);
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
    GREEN1 = 1;
}

void i2cTesting(void) {
    
    I2Cinitializers();
    
    while (1) {
        HPread();
        sprintf(message, "%.2f, %.2f", HPgetTemperature(), HPgetPressure());
        println(message);
        delay(1000, MILLI);
    }
}

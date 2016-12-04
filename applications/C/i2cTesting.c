#include "../include/i2cTesting.h"

//char message[100];

void I2Cinitializers(void) {
    
    // hardware
    __builtin_disable_interrupts();
    
    initUART();
    I2Cinit(1, 400, true);
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
    GREEN1 = 1;
}

void i2cTesting(void) {
    
    I2Cinitializers();
    
    testDisplay();
    
    while (1) {
        
        printf("%s\r\n", testDisplay ? "worked" : "did not work");
        
        YELLOW1 = 1;
        delay(500, MILLI);
        YELLOW1 = 0;
        delay(500, MILLI);
    }
}

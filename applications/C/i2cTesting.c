#include "../include/i2cTesting.h"

char message[255];
uint32_t receive[4];

void I2Cinitializers(void) {
    
    // hardware
    __builtin_disable_interrupts();
    
    initializeTimer1(0x8000, 0xffff);
    initLEDs();
    initUART();
    I2Cinit(1, 100, true);
    initADC();
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    blinkBoardLights(5, 100);
    
    CAN_init();
}

void i2cTesting(void) {
    
    I2Cinitializers();
    
    while (1) {
        
        delay(1000, MILLI);
    }
}

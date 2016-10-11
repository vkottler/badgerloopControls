#include <xc.h>
#include "main.h"

void initializers(void) {
    
    // hardware
    DDPCONbits.JTAGEN = 0;
    __builtin_disable_interrupts();
    
    initializeTimer1(0x8000, 0xffff);
    initLEDs();
    //initUART();
    I2Cinit();
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    blinkBoardLights(5, 100);
    
    // individual peripherals
    VL_init(VL_ADDRESS);
}

int main(void) {
    
    initializers();
    
    while (1) {
        GREEN1 = 1;
        VL_sample(VL_ADDRESS);
        //printByteln(VL_sample(VL_ADDRESS));
        delay(1000, MILLI);
        GREEN1 = 0;
        delay(1000, MILLI);
    }
    
    return 0;
}

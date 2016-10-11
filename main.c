#include <xc.h>
#include "main.h"

void initializers(void) {
    
    // hardware
    DDPCONbits.JTAGEN = 0;
    __builtin_disable_interrupts();
    
    initializeTimer1(0x8000, 0xffff);
    initLEDs();
    initUART();
    //I2Cinit();
    initADC();
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    blinkBoardLights(5, 100);
    
    // individual peripherals
    //VL_init(VL_ADDRESS);
}

int i;
char message[100];

int main(void) {
    
    initializers();
    
    while (1) {
        
        for (i = 0; i < 3; i++) {
            sprintf(message, "%d: %.3f ", i, toVolts(analogRead(i)));
            print(message);
        }
        println("");
        
        BOARD_LED2 = 1;
        delay(500, MILLI);
        BOARD_LED2 = 0;
        delay(500, MILLI);
    }
    
    return 0;
}

// Prox usage: VL_sample(VL_ADDRESS);

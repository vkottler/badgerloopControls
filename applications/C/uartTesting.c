#include "../include/uartTesting.h"

char message[50];

void uartInitializers(void) {
    
    // hardware
    __builtin_disable_interrupts();
    
    initUART();
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    
    GREEN1 = 1;
}

void uartTesting(void) {
    
    uartInitializers();
    
    while (1) {
        
        println("Heartbeat.");
        if (messageAvailable()) {
            getMessage(message, 50);
            println(message);
        }
        
        YELLOW1 = 1;
        delay(500, MILLI);
        YELLOW1 = 0;
        delay(500, MILLI);
    }
}
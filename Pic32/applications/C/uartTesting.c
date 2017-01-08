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
        
        printf("Heartbeat.\r\n");
        if (messageAvailable()) {
            getMessage(message, 50);
            printf(message); printf("\r\n");
        }
        
        YELLOW1 = 1;
        delay(500, MILLI);
        YELLOW1 = 0;
        delay(500, MILLI);
    }
}
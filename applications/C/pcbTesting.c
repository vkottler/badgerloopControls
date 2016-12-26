#include "../include/pcbTesting.h"

char message[50];

void initializers(void) {
    __builtin_disable_interrupts();
    initUART();
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
}

void testCAN(void) {
    
}

void testVNM(void) {
    
    printf("\r\nTesting VNM . . .\r\n");
    
    while (1) {
        GREEN1 = 1;
        delay(500, MILLI);
        GREEN1 = 0;
        delay(500, MILLI);
        
    }
}

void testMCM(void) {
    
    printf("\r\nTesting MCM . . .\r\n");
    
    while (1) {
        GREEN1 = 1;
        delay(500, MILLI);
        GREEN1 = 0;
        delay(500, MILLI);
        
    }
}

void testBCM(void) {
    
    printf("\r\nTesting BCM . . .\r\n");
    
    while (1) {
        GREEN1 = 1;
        delay(500, MILLI);
        GREEN1 = 0;
        delay(500, MILLI);
        
    }
}

void testPCBs(void) {
    initializers();
    
    printf("SOFTWARE LOADED: PCB Testing\r\n\r\n");
    printf("Please type 'MCM', 'BCM', or 'VNM' to begin: ");
    
    while (1) {
        if (messageAvailable()) {
            getMessage(message, 50);
            if (!strcmp(message, "MCM")) testMCM();
            else if (!strcmp(message, "BCM")) testBCM();
            else if (!strcmp(message, "VNM")) testVNM();
            else printf("Did not recognize %s.\r\n", message);
        }
    }
}

#include <xc.h>
#include "main.h"

#define MAX_LENGTH      100

char message[MAX_LENGTH];

void initializers(void) {
    initLEDs();
    initializeTimer1(0x8000, 0xFFFF);
    initUART();
    
    // Print Board Info
    printMAC(message);
    printBoardNumber(message);
}

int main(void) {
    
    initializers();
    waitForButton();
    
    
    while (1) {
        if (UARTavailable()) {
            getMessage(message, MAX_LENGTH);
            blinkBoardLights(5, 100);
            println(message);
        }
        println("Heartbeat.");
        delay(1000, MILLI);
    }
    return 0;
}
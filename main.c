#include <xc.h>
#include "main.h"

#define MAX_LENGTH      100

void initializers(void) {
    initLEDs();
    initializeTimer1(0x8000, 0xFFFF);
    initUART();
}

int main(void) {
    
    char incoming[MAX_LENGTH];
    
    initializers();
    
    while (1) {
        if (UARTavailable()) {
            getMessage(incoming, MAX_LENGTH);
            blinkBoardLights(5, 100);
            println(incoming);
        }
        println("Heartbeat.");
        delay(1000, MILLI);
    }
    return 0;
}
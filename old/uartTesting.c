#include "../include/uartTesting.h"

char message[50];

void uartTesting(void) {
    while (1) {
        
        //printf("Heartbeat.\r\n");
        if (messageAvailable()) {
            getMessage(message, 50);
            printf("%s\r\n", message);
        }
        blinkBoardLights(5, 100);
    }
}

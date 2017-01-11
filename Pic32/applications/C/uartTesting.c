#include "../include/uartTesting.h"

char message[50];

void uartTesting(void) {
    GREEN1 = 1;
    while (1) {
        
        //printf("Heartbeat.\r\n");
        if (messageAvailable()) {
            getMessage(message, 50);
            printf("%s\r\n", message);
        }
        YELLOW1 = 1;
        delay(500, MILLI);
        YELLOW1 = 0;
        delay(500, MILLI);
    }
}

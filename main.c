#include <xc.h>
#include "main.h"

#define MAX_LENGTH      100

char message[MAX_LENGTH];

void initializers(void) {
    __builtin_disable_interrupts();
    DDPCONbits.JTAGEN = 0;
    initLEDs();
    initializeTimer1(0x8000, 0xffff);
    initializeSlowTimer(1000);
    //initUART();
    
    // Print Board Info
    //printMAC(message);
    //printBoardNumber(message);
    
    // Enable Interrupts
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
}

int main(void) {
    
    int curr = 0;
    
    initializers();
    //waitForButton();
    
    while (1) {
        
        //if (UARTavailable()) {
        //    getMessage(message, MAX_LENGTH);
        //    blinkBoardLights(5, 100);
        //    println(message);
        //}
        
        if (events) {
            if (events && TIMER_5_BIT) {
                BOARD_LED1 = curr;
                curr ^= 0x1;
                BOARD_LED2 = curr;
                events &= TIMER_5_MASK;
            }
        }
    }
    return 0;
}
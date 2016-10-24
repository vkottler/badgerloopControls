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
    //initADC();
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    blinkBoardLights(5, 100);
    
    // individual peripherals
    //VL_init(VL_ADDRESS);
    CAN_init();
}

int i;
int val;
char message[100];


uint32_t toSend[4];
uint32_t receive[4];
unsigned int * addr;

int main(void) {
    
    initializers();
    
    while (1) {
        
        print("Enter an integer to send: ");
        while (!UARTavailable());
        getMessage(message, 100);
        val = atoi(message);
        sprintf(message, "Sending: %d", val);
        println(message);
        
        //toSend[0] = ID_FOR_KELLY;
        //toSend[1] = sizeof(val);
        //toSend[2] = val;
        //toSend[3] = 0;
        //CAN_send_message(toSend);
        
        addr = PA_TO_KVA1(C1FIFOUA1);
        addr[0] = ID_FOR_KELLY;
        addr[1] = sizeof(val);
        addr[2] = val;
        C1FIFOCON1SET = 0x2000;
        C1FIFOCON1bits.TXREQ = 1;
        
        //CAN_receive_message(receive);
        YELLOW2 = 1; while (!C1FIFOINT0bits.RXNEMPTYIF); YELLOW2 = 0;
        addr = PA_TO_KVA1(C1FIFOUA0);
        C1FIFOCON0SET = 0x2000;
        
        //sprintf(message, "Received %d with SID = 0x%x, size = %d", receive[2], receive[0] & 0x7FF, receive[1]);
        sprintf(message, "Received %d with SID = 0x%x, size = %d", addr[2], addr[0] & 0x7FF, addr[1]);
        
        println(message);
                
        BOARD_LED2 = 1;
        delay(500, MILLI);
        BOARD_LED2 = 0;
        delay(500, MILLI);
    }
    
    return 0;
}

// Prox usage: VL_sample(VL_ADDRESS);

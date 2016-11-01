#include "usbUART.h"

char receiveBuffer[255];
volatile uint8_t UARTstatus = 0;

void initUART() {
    U1MODEbits.BRGH = 1;
    U1BRG = BRATE;
    
    // initialize TX and RX
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    
    // Generate Interrupt when receive buffer not empty
    U1STAbits.URXISEL = 0;
    IEC0bits.U1RXIE = 1; // enable interrupt 
    IFS0bits.U1RXIF = 0; // clear flag
    IPC6bits.U1IP = 1; // priority 1
    IPC6bits.U1IS = 1; // sub-priority 1
    
    // NEEDS TO BE LAST
    U1MODEbits.ON = 1;
}

int UARTavailable() { return UARTstatus & DATA_READY; }

void getMessage(char *message, int maxLength) {
    int index = 0;
    while (receiveBuffer[index] != '\0' && index < maxLength) {
        message[index] = receiveBuffer[index];
        index++;
    }
    message[index] = '\0';
    UARTstatus |= DATA_READ;
    UARTstatus &= ~DATA_READY;
}

void print(const char *string) {
    while (*string != '\0') {
        while (U1STAbits.UTXBF);
        U1TXREG = *(string++);
    }
}

void println(const char *string) { print(string); print("\r\n"); }
void printByteln(uint8_t byte) { printByte(byte); print("\r\n"); }

void printByte(uint8_t byte) {
    char first = '\0';
    char second = '\0';
    int hex1 = (byte & 0xf0) >> 4;
    int hex2 = byte & 0x0f;
    if (hex1 > 9) {
        switch (hex1) {
            case 10: first = 'A'; break;
            case 11: first = 'B'; break;
            case 12: first = 'C'; break;
            case 13: first = 'D'; break;
            case 14: first = 'E'; break;
            case 15: first = 'F'; break;
            default: first = 'X';
        }
    }
    else first = hex1 + 48;
    if (hex2 > 9) {
        switch(hex2) {
            case 10: second = 'A'; break;
            case 11: second = 'B'; break;
            case 12: second = 'C'; break;
            case 13: second = 'D'; break;
            case 14: second = 'E'; break;
            case 15: second = 'F'; break;
            default: second = 'X';
        }
    }
    else second = hex2 + 48;
    char toSend[] = {'0', 'x', first, second, '\0'};
    print(toSend);
}

void __ISR(UARTvec, IPL1SOFT) receiveHandler(void) {
    
    static int receiveIndex = 0;
    
    if (UARTstatus & DATA_READ) {
        receiveIndex = 0;
        UARTstatus &= ~DATA_READ;
    }
    
    if (U1STAbits.URXDA) {
        
        char data = U1RXREG;
        
        // just throw out new UART if we didn't read old message
        if (!(UARTstatus & DATA_READY)) {
            if (data == '\n') {
                receiveBuffer[receiveIndex] = '\0';
                UARTstatus |= DATA_READY;
            }
            else if (data != '\r') {
                receiveBuffer[receiveIndex] = data;
                receiveIndex++;
            }
        }
    }
    IFS0bits.U1RXIF = 0;
}

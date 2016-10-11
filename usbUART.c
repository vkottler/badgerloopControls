#include "usbUART.h"

void initUART() {
    U1MODEbits.BRGH = 1;
    U1BRG = BRATE;
    
    // initialize TX and RX
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    
    // NEEDS TO BE LAST
    U1MODEbits.ON = 1;
}

int UARTavailable() { return U1STAbits.URXDA; }

void getMessage(char *message, int maxLength) {
    char data = 0;
    short complete = 0, num_bytes = 0;
    while (!complete) {
        if (U1STAbits.URXDA) {
            data = U1RXREG;
            if ((data == '\n')) complete = 1;
            else if (data != '\r') {
                message[num_bytes++] = data;
                if (num_bytes >= maxLength) num_bytes = 0;
            }
        }
    }
    message[num_bytes] = '\0';
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

void checkSerialErrors() {
    if (U1STAbits.PERR) setBoardLight(0, 1);
    if (U1STAbits.FERR) setBoardLight(1, 1);
    if (U1STAbits.OERR) setBoardLight(2, 1);
}

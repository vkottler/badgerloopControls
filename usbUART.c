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

void println(const char *string) {
    print(string);
    print("\r\n");
}

void checkSerialErrors() {
    if (U1STAbits.PERR) setBoardLight(0, 1);
    if (U1STAbits.FERR) setBoardLight(1, 1);
    if (U1STAbits.OERR) setBoardLight(2, 1);
}

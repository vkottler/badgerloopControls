#include "../include/usbUART.h"

PC_Buffer tx_buffer, rx_buffer;
volatile uint8_t availableCount = 0;

void initUART() {
    
    // Initialize produce/consume buffers
    pc_buffer_init(&tx_buffer, BUFSIZ);
    pc_buffer_init(&rx_buffer, BUFSIZ);
    
    U1MODEbits.BRGH = 1;
    U1BRG = BRATE;
    
    // initialize TX and RX
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    
    // Generate Interrupt when receive buffer not empty
    U1STAbits.URXISEL = 0;          // int generated when recieve buffer not empty
    U1STAbits.UTXISEL = 0;          // int generated when transmit buffer has at least one empty space
    IEC0bits.U1RXIE = 1;            // enable interrupt 
    USB_RX_FLAG = 0;                // clear flag
    USB_TX_FLAG = 0;
    IPC6bits.U1IP = 1;              // priority 1
    IPC6bits.U1IS = 1;              // sub-priority 1
    
    // NEEDS TO BE LAST
    U1MODEbits.ON = 1;
}

bool messageAvailable(void) { return availableCount > 0; }

void getMessage(char *message, int maxLength) {
    char curr;
    int index = 0;
    
    while (index < maxLength && !pc_buffer_empty(&rx_buffer)) {
        pc_buffer_remove(&rx_buffer, &curr);
        if (curr != '\r') message[index++] = (curr == '\n') ? '\0' : curr;
        if (curr == '\n') {
            availableCount--;
            return;
        }
    }
}

void print(const char *string) {
    while (*string != '\0' && !pc_buffer_full(&tx_buffer)) 
        pc_buffer_add(&tx_buffer, *(string++));
    USB_TX_EN = 1;
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

void __ISR(U1vec, IPL1SOFT) receiveHandler(void) {
    
    static char curr;
    
    // Handle received char
    if (USB_RX_FLAG) {
        curr = U1RXREG;
        if (curr == '\n') availableCount++;
        if (!pc_buffer_full(&rx_buffer))
            pc_buffer_add(&rx_buffer, curr);
        USB_RX_FLAG = 0;
    }
    
    // Handle sending a char
    if (USB_TX_FLAG) {
        while (!U1STAbits.UTXBF && !pc_buffer_empty(&tx_buffer))           // read from the pc buffer until the hardware buffer is full, or until there are no more characters
            pc_buffer_remove(&tx_buffer, (char *) &U1TXREG);
        if (pc_buffer_empty(&tx_buffer)) USB_TX_EN = 0;                    // if there aren't any characters to send disable the interrupt
        USB_TX_FLAG = 0;
    }
}

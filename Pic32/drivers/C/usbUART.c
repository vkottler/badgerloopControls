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
    
    if (!messageAvailable()) return;
    
    while (index < maxLength && !pc_buffer_empty(&rx_buffer)) {
        pc_buffer_remove(&rx_buffer, &curr);
        if (curr != '\r') message[index++] = (curr == '\n') ? '\0' : curr;
        if (curr == '\n') {
            availableCount--;
            return;
        }
    }
}

// Deprecated
void print(const char *string) { printf(string); }
void println(const char *string) { printf(string); printf("\r\n"); }

void _mon_putc(char c) {
    while (pc_buffer_full(&tx_buffer));
    __builtin_disable_interrupts();
    pc_buffer_add(&tx_buffer, c);
    __builtin_enable_interrupts();
    USB_TX_EN = 1;
}

void __ISR(U1vec, IPL1SOFT) receiveHandler(void) {
    
    static char curr;
    
    __builtin_disable_interrupts();
    
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
    
    __builtin_enable_interrupts();
}
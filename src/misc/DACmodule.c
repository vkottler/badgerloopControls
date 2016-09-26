#include "DACmodule.h"

int error = 0;
uint16_t toSend = 0x00;

void initSPI(void) {
    
    // 16 bit data frames
    SPI2CONbits.MODE16 = 1;
    SPI2CONbits.MODE32 = 0;
    
    // in idle, clock line should be high
    SPI2CONbits.CKP = 1;
    SPI2STATbits.SPIROV = 0; // clear ovcerflow bit for some reason?
    SPI2BRG = 130;           // if my calculation is correct, this should be 500kbps (63)
    SPI2CONbits.MSTEN = 1;  // pic32 operates as master
    SPI2CONbits.ON = 1;
}

void sendDACsignal(uint16_t value, uint8_t address) {
    if (value < 0 || value > 1023) { RED2 = 1; return; error += 1; }
    if (address < 1 || address > 10) { RED2 = 1; return; error += 2;}
    toSend = ((value << 4) + (address & 0x0f));
    while (SPI2STATbits.SPIBUSY || SPI2STATbits.SPITBF);
    SPI2BUF = toSend;
}

int checkDACError(void) { return error; }

void printDACError(void) {
    switch (error) {
        case 1:
            println("Value to send was not between 1 and 1023.");
            break;
        case 2:
            println("Invalid address.");
            break;
        case 3:
            println("Invalid address and data argument.");
            break;
    }
    error = 0;
}
#include "../include/SPI.h"

void initSPI(void) {
    SPI2ACONbits.MSTEN = 1;
    SPI2ACONbits.SMP = 1;
    
    
    
    SPI2ACONbits.ON = 1;
}

void SPIsendByte(uint8_t byte) {
    while (SPI2ASTATbits.SPIBUSY);
    SPI2ABUF = byte;
}

void SPIsendBytes(uint8_t bytes, int size) {
    int i = 0;
    for (i = 0; i < size; i++) SPIsendByte(bytes + i);
}

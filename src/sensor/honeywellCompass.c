#include "honeywellCompass.h"

bool dataReady = false;

int initCompass(void) {
    //INTCONbits.MVEC = 1;        // allow multi vector interrupts
    //INTCONbits.INT1EP = 0;      // falling edge
    //IFS0bits.INT1IF = 0;        // set the flag to 0
    //IPC1bits.INT1IP = 1;        // priority 1
    //IEC0bits.INT1IE = 1;        // enable INT1 pin interrupt
    
    // Write Register A
    I2CwriteByteTo8bitRegister(COMPASS_ADDR, 0x00, 0x70);
    
    // Leave Register B alone, Write Mode Register
    I2CwriteByteTo8bitRegister(COMPASS_ADDR, 0x02, 0x00);
    
    return I2CcheckError();
}

bool checkDataReady(void) { return dataReady; }

int CompassGetData(uint8_t *array) {
    //while(!dataReady);
    //dataReady = false;
    I2CCompassReadRegisters(0x3D, 0x06, array);
    // set the pointer back to register 3
    I2CSendTwoBytes(0x3C, 0x03);
    return I2CcheckError();
}

void __ISR (7, ipl1SOFT) INT1Interrupt (void) {
    dataReady = true;
    IFS0bits.INT1IF = 0;
}
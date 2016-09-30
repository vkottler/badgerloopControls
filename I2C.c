#include "I2C.h"

uint8_t I1CTXBuffer[255];
uint8_t I1CRXBuffer[255];
char message[255];

I2CError checkError = none;

void I2CInit() {
    // we don't need to adjust any settings
    I2C1BRG = BRGVAL400k;
    I2C1CONbits.ON = 1;  
}

void I2CDisable() { I2C1CONbits.ON = 0; }

bool I2CCheckTimeout() {
    if (_CP0_GET_COUNT() > I2C_TIMEOUT) return true;
    return false;
}

void IdleI2C1(void) {
    _CP0_SET_COUNT(0);
    while((I2C1CONbits.SEN || I2C1CONbits.PEN || I2C1CONbits.RCEN || I2C1CONbits.ACKEN || I2C1STATbits.TRSTAT) && !I2CCheckTimeout());
    if (I2CCheckTimeout()) {
        RED1 = 1;
        if (I2C1CONbits.SEN) checkError = startTO;
        if (I2C1CONbits.PEN) checkError = stopTO;
        if (I2C1CONbits.RCEN) checkError = receivingTO;
        if (I2C1CONbits.ACKEN) checkError = ackTO;
        if (I2C1STATbits.TRSTAT) checkError = sendingTO;
    }
    else { checkError = none; RED1 = 0; }
}

int I2CcheckError(void) { return checkError; }

void I2CprintError(void) {
    switch (checkError) {
        case none:
            println("I2C: no errors.");
            break;
        case buscol:
            println("I2C: Bus/write collision during write.");
            break;
        case startTO:
            println("I2C: Start condition timeout.");
            break;
        case stopTO:
            println("I2C: Stop condition timeout.");
            break;
        case receivingTO:
            println("I2C: Receiving Byte timed out.");
            break;
        case sendingTO:
            println("I2C: Sending Byte timed out.");
            break;
        case ackTO:
            println("I2C: Sending ACK timed out.");
            break;
        case slaveNoResponse:
            println("I2C: Slave didn't acknowledge.");
            break;
    }
}

void I2CStart() {
    //IdleI2C1();       // this could waste computation time, if we need to use it later we can
    _CP0_SET_COUNT(0);
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN && !I2CCheckTimeout());
    if (I2CCheckTimeout()) checkError = startTO;
    else checkError = none;
}

void I2CStop() {
    _CP0_SET_COUNT(0);
    I2C1CONbits.PEN = 1;            // initiate stop on SDA/SCL
    while (I2C1CONbits.PEN && !I2CCheckTimeout());
    if (I2CCheckTimeout()) checkError = stopTO;
    else checkError = none;
}

void I2CRepeatedStart() {     
    I2C1CONbits.RSEN = 1;
    _CP0_SET_COUNT(0);
    while (I2C1CONbits.RSEN && !I2CCheckTimeout());
    if (I2CCheckTimeout()) checkError = startTO;
    else checkError = none;
}

void I2CSendByte(uint8_t* data) {
    IdleI2C1();
    _CP0_SET_COUNT(0);
    while (I2C1STATbits.TBF);                                           // wait if the transmit buffer is full
    if (I2C1STATbits.BCL || I2C1STATbits.IWCOL) {
        checkError = buscol;
        return; // it literally won't do anything, just give up
    }
    I2C1TRN = *data;
    while (I2C1STATbits.TRSTAT && !I2CCheckTimeout());                  // wait until we're done sending
    if (I2CCheckTimeout()) checkError = sendingTO;
    else checkError = none;
}

bool checkACK() { return !I2C1STATbits.ACKSTAT; } // we WANT a 0 (ACK rec)
bool checkNACK() { return I2C1STATbits.ACKSTAT; } // we WANT a 1 (NACK rec)

void sendACK() {
    _CP0_SET_COUNT(0);
    I2C1CONbits.ACKDT = 0;          // prepare to send ACK 
    I2C1CONbits.ACKEN = 1;          // send ACKDT
    while (I2C1CONbits.ACKEN && !I2CCheckTimeout());
    if (I2CCheckTimeout()) checkError = ackTO;
    else checkError = none;
}

void sendNACK() {
    _CP0_SET_COUNT(0);
    I2C1CONbits.ACKDT = 1;          // prepare to send ACK 
    I2C1CONbits.ACKEN = 1;          // send ACKDT
    while (I2C1CONbits.ACKEN && !I2CCheckTimeout());
    if (I2CCheckTimeout()) checkError = ackTO;
    else checkError = none;
}

uint8_t I2CReceiveByte(void) {
    IdleI2C1();
    _CP0_SET_COUNT(0);
    I2C1CONbits.RCEN = 1; 
    while (I2C1CONbits.RCEN && !I2CCheckTimeout());
    if (I2CCheckTimeout()) checkError = receivingTO;
    if (!I2C1STATbits.RBF) checkError = receivingTO;
    uint8_t retval;
    retval = I2C1RCV;
    return retval;
}

void I2CRequestFrom(uint8_t deviceAddress, int numBytes, uint8_t* data) {
    I1CTXBuffer[0] = (deviceAddress << 1) + 1; // add one to signify read
    I2CStart();
    I2CSendByte(I1CTXBuffer);
    if (!checkACK()) checkError = slaveNoResponse;
    short i = 0;
    for (i = 0; i < numBytes; i++) {
        data[i] = I2CReceiveByte();
        if (i < numBytes - 1) sendACK();
    }
    sendNACK();
    I2CStop();
}

void I2CwriteByteToRegister(uint8_t deviceAddress, uint16_t deviceRegister, uint8_t data) {
    uint8_t i = 0;
    I1CTXBuffer[0] = deviceAddress << 1;
    I1CTXBuffer[1] = (deviceRegister & 0xff00) >> 8;
    I1CTXBuffer[2] = deviceRegister & 0x00ff;
    I1CTXBuffer[3] = data;
    I2CStart();
    for (i = 0; i < 4; i++) {
        I2CSendByte((I1CTXBuffer+i));
        if (!checkACK()) checkError = slaveNoResponse;
    }
    I2CStop();
}

void I2CwriteByteTo8bitRegister(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t data) {
    int i = 0;
    I1CTXBuffer[0] = deviceAddress << 1;
    I1CTXBuffer[1] = deviceRegister;
    I1CTXBuffer[2] = data;
    I2CStart();
    for (i = 0; i < 3; i++) {
        I2CSendByte((I1CTXBuffer+i));
        if (!checkACK()) checkError = slaveNoResponse;
    }
    I2CStop();
}

uint8_t I2CReadByteFromRegister(uint8_t deviceAddress, uint16_t deviceRegister) {
    uint8_t i = 0;
    I1CTXBuffer[0] = deviceAddress << 1;
    I1CTXBuffer[1] = (deviceRegister & 0xff00) >> 8;
    I1CTXBuffer[2] = deviceRegister & 0x00ff;
    I2CStop();
    I2CStart();
    for (i = 0; i < 3; i++) {
        I2CSendByte((I1CTXBuffer+i));
        if (!checkACK()) checkError = slaveNoResponse;
    }
    I2CStop();
    I1CTXBuffer[0] = (deviceAddress << 1) + 1; // set read indication
    I2CStart();
    I2CSendByte(I1CTXBuffer); 
    if (!checkACK()) checkError = slaveNoResponse;
    i = I2CReceiveByte();
    sendNACK();
    I2CStop();
    return i;
}

void I2CReadConsecutiveRegisters(uint8_t deviceAddress, uint16_t deviceRegister, uint8_t numBytes, uint8_t *data) {
    uint8_t i = 0;
    I1CTXBuffer[0] = deviceAddress << 1;
    I1CTXBuffer[1] = (deviceRegister & 0xff00) >> 8;
    I1CTXBuffer[2] = deviceRegister & 0x00ff;
    I2CStart();
    for (i = 0; i < 3; i++) {
        I2CSendByte((I1CTXBuffer+i));
        if (!checkACK()) checkError = slaveNoResponse;
    }
    I2CStop();
    //delay(10);
    I2CStart();
    I1CTXBuffer[0] = I1CTXBuffer[0] + 1; // read indication
    I2CSendByte(I1CTXBuffer);
    if (!checkACK()) checkError = slaveNoResponse;
    for (i = 0; i < numBytes; i++) {
        *(data + i) = I2CReceiveByte();
        if (i < numBytes - 1) sendACK();
    }
    sendNACK();
    I2CStop();
}

void I2CwriteWordToRegister(uint8_t deviceAddress, uint16_t deviceRegister, uint16_t data) {
    uint8_t i = 0;
    I1CTXBuffer[0] = deviceAddress << 1;
    I1CTXBuffer[1] = (deviceRegister & 0xff00) >> 8;
    I1CTXBuffer[2] = deviceRegister & 0x00ff;
    I1CTXBuffer[3] = (data & 0xff00) >> 8;
    I1CTXBuffer[4] = data & 0x00ff;
    I2CStart();
    for (i = 0; i < 5; i++) {
        I2CSendByte((I1CTXBuffer+i));
        if (!checkACK()) checkError = slaveNoResponse;
    }
    I2CStop();
}

void I2CSendTwoBytes(uint8_t byte1, uint8_t byte2) {
    I1CTXBuffer[0] = byte1;
    I1CTXBuffer[1] = byte2;
    I2CStart();
    I2CSendByte(I1CTXBuffer);
    if (!checkACK()) checkError = slaveNoResponse;
    I2CSendByte((I1CTXBuffer+1));
    if (!checkACK()) checkError = slaveNoResponse;
    I2CStop();
}

void I2CCompassReadRegisters(uint8_t byte1, uint8_t byte2, uint8_t *data) {
    int i = 0;
    I1CTXBuffer[0] = byte1;
    I1CTXBuffer[1] = byte2;
    I2CStart();
    I2CSendByte(I1CTXBuffer);
    if (!checkACK()) checkError = slaveNoResponse;
    I2CSendByte((I1CTXBuffer+1));
    if (!checkACK()) checkError = slaveNoResponse;
    for (i = 0; i < 6; i++) {
        *(data+i) = I2CReceiveByte();
        sendNACK();
    }
    I2CStop();
}
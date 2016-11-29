#include "../include/I2C.h"

// for keeping track of state during the ISR
static volatile enum { IDLE, START, WRITE, READ, RESTART, ACK, NACK, STOP, ERROR } state = IDLE;

static buffer_t toWrite = NULL, toRead = NULL;                          // shared read & write buffers
static volatile uint8_t address = 0;                              // 7 bit address
static volatile unsigned int numWrite = 0, numRead = 0;                 // used by ISR to decide how many times to ____

void I2Cinit(void) {
    I2C1BRG = ONE_HUNDRED;               // 400kHz mode
    IPC6bits.I2C1IP = 1;        // master int priority 1
    IEC0bits.I2C1MIE = 1;       // master int enable
    IFS0bits.I2C1AMIF = 0;      // disable flag
    I2C1CONbits.ON = 1;
}

int I2CwriteAndRead(unsigned int addr, const buffer_t write, unsigned int wlen, const buffer_t read, unsigned int rlen) {
    numWrite = wlen, numRead = rlen;
    toWrite = write, toRead = read;
    address = addr;
    state = START;
    I2C1CONbits.SEN = 1;
    
    // Hopefully this never happens
    while(state != IDLE && state != ERROR) 
        if (I2C1STATbits.BCL) return 1;
    
    return state == ERROR;
}

void __ISR(I2Cvec, IPL1SOFT) I2CmasterInt(void) {
    static unsigned int writeIndex = 0, readIndex = 0;                          // keep value between invocations
    
    switch (state) {
        
        // start bit was sent
        case START:
            writeIndex = 0, readIndex = 0;
            if (numWrite > 0) { state = WRITE; I2C1TRN = address << 1; } 
            else { state = ACK; I2C1TRN = (address << 1) | 1; }                 // we wanted to read
            break;
            
        // start done
        case WRITE:
            if (I2C1STATbits.ACKSTAT) state = ERROR;                            // we didn't receive ACK
            else {
                if (writeIndex < numWrite) I2C1TRN = toWrite[writeIndex++];     // keep writing
                else {
                    if (numRead > 0) { state = RESTART; I2C1CONbits.RSEN = 1; } // reading instead (start again)
                    else { state = STOP; I2C1CONbits.PEN = 1; }                 // done
                }
            }
            break;
        
        // finished ack
        case READ:
            if (I2C1STATbits.ACKSTAT) state = ERROR;                            // didn't receive ACK
            else {
                toRead[readIndex++] = I2C1RCV;
                if (readIndex == numRead) { state = NACK; I2C1CONbits.ACKDT = 1; }  // done reading
                else { state = ACK; I2C1CONbits.ACKDT = 0; }                        // not done so acknowledge
                I2C1CONbits.ACKEN = 1;
            }
            break;
             
        case RESTART: state = ACK; I2C1TRN = (address << 1) | 1;  break;        // restart complete  
        case ACK: state = READ; I2C1CONbits.RCEN = 1; break;                    // ack finished sending, re-enable receive
        case NACK: state = STOP; I2C1CONbits.PEN = 1; break;                    // just stop, no more incoming data
        case STOP: state = IDLE; break;                                         // stop sequence (SEN HW clear) finished, return
        default: state = ERROR;                                                 // we should never get here       
    }
    IFS0bits.I2C1MIF = 0;
}

#ifndef _I2C__H__
#define _I2C__H__

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>
#include <stdio.h>
#include "ledShield.h"
#include "usbUART.h"

/*
 * Potential I2C Interrupt Vectors:
 * 
 * I2C1B: Bus Collision 29 25 IFS0<29> IEC0<29> IPC6<12:10> IPC6<9:8>
 * I2C1S: Slave Event   30 25 IFS0<30> IEC0<30> IPC6<12:10> IPC6<9:8>
 * I2C1M: Master Event  31 25 IFS0<31> IEC0<31> IPC6<12:10> IPC6<9:8>
 */
#define I2Cvec      25

// "buffer pointer" buffer contents and buffer pointer
// is shared by ISR and main code execution (i.e. double volatile)
typedef volatile uint8_t * volatile buffer_t;

void I2Cinit(void);
void I2CmasterInt(void);
int I2CwriteAndRead(unsigned int addr, const buffer_t write, unsigned int wlen, const buffer_t read, unsigned int rlen);

#endif

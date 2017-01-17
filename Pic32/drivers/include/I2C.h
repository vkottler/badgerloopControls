#ifndef _I2C__H__
#define _I2C__H__

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/attribs.h>
#include <stdio.h>

/*
 * Potential I2C Interrupt Vectors:
 * 
 * I2C1B: Bus Collision 29 25 IFS0<29> IEC0<29> IPC6<12:10> IPC6<9:8>
 * I2C1S: Slave Event   30 25 IFS0<30> IEC0<30> IPC6<12:10> IPC6<9:8>
 * I2C1M: Master Event  31 25 IFS0<31> IEC0<31> IPC6<12:10> IPC6<9:8>
 * 
 * I2C2B: Bus Collision 43 33 IFS1<11> IEC0<11> IPC<12:10> IPC<9:8>
 * I2C2S: Slave Event   44 33 IFS1<12> IEC0<12> IPC<12:10> IPC<9:8>
 * I2C2M: Master Event  45 33 IFS1<13> IEC0<13> IPC<12:10> IPC<9:8>
 * 
 * I2C3B: Bus Collision 26 24 IFS0<26> IEC0<26> IPC<4:2> IPC<1:0>
 * I2C3S: Slave Event   27 24 IFS0<27> IEC0<27> IPC<4:2> IPC<1:0>
 * I2C3M: Master Event  28 24 IFS0<28> IEC0<28> IPC<4:2> IPC<1:0>
 * 
 * I2C4B: Bus Collision 37 31 IFS1<5> IEC1<5> IPC<28:26> IPC<25:24>
 * I2C4S: Slave Event   38 31 IFS1<6> IEC1<6> IPC<28:26> IPC<25:24>
 * I2C4M: Master Event  39 31 IFS1<7> IEC1<7> IPC<28:26> IPC<25:24>
 * 
 * I2C5B: Bus Collision 40 32 IFS0<8> IEC0<8> IPC<4:2> IPC<1:0>
 * I2C5S: Slave Event   41 32 IFS0<9> IEC0<9> IPC<4:2> IPC<1:0>
 * I2C5M: Master Event  42 32 IFS0<10> IEC0<10> IPC<4:2> IPC<1:0>
 */
#define I2C1vec             25

#define FOUR_HUNDRED        78
#define ONE_HUNDRED         318

#define I2C_BAUD            FOUR_HUNDRED                

// "buffer pointer" buffer contents and buffer pointer
// is shared by ISR and main code execution (i.e. double volatile)
typedef volatile uint8_t * volatile buffer_t;

void I2Cinit(void);
void I2CmasterInt(void);
bool I2CwriteAndRead(unsigned int addr, const buffer_t write, unsigned int wlen, const buffer_t read, unsigned int rlen);

#endif

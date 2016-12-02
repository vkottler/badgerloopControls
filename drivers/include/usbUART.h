#ifndef _USBUART__H__
#define _USBUART__H__

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/attribs.h>

#include "../../peripherals/include/pc_buffer.h"

// 115200 baud, 8 bit 1 stop no parity

// Settings
// bit 15 - on (0x8000), bit 3 - BRGH (0x0004) 
#define BRATE           138 // see BRG equation to see how we got here

// U1RX 27 24 IFS0<27> IEC0<27> IPC6<4:2> IPC6<1:0>
// U1TX 28 24 IFS0<28> IEC0<28> IPC6<4:2> IPC6<1:0>
#define U1vec           24

#define DATA_READY      1
#define DATA_READ       2

#define USB_RX_FLAG    IFS0bits.U1RXIF
#define USB_TX_FLAG    IFS0bits.U1TXIF
#define USB_TX_EN      IEC0bits.U1TXIE

void initUART(void);
bool messageAvailable(void);
void getMessage(char *message, int maxLength);
void print(const char *string);
void println(const char *string);
void printByte(uint8_t byte);
void printByteln(uint8_t byte);

#endif

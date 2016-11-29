#ifndef _USBUART__H__
#define _USBUART__H__

#include <xc.h>
#include <stdint.h>
#include <sys/attribs.h>
// 115200 baud, 8 bit 1 stop no parity

// Settings
// bit 15 - on (0x8000), bit 3 - BRGH (0x0004) 
#define BRATE           138 // see BRG equation to see how we got here

// Flag:            IFS0<27>
// Enable:          IEC0<27>
// Priority:        IPC6<4:2>
// Sub-Priority:    IPC6<1:0>
#define UARTvec         24

#define DATA_READY      1
#define DATA_READ       2

void initUART(void);
int UARTavailable(void);
void getMessage(char *message, int maxLength);
void print(const char *string);
void println(const char *string);
void printByte(uint8_t byte);
void printByteln(uint8_t byte);

#endif

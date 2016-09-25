#include <xc.h>
#include "initialize.h"
// 115200 baud, 8 bit 1 stop no parity

// Settings
// bit 15 - on (0x8000), bit 3 - BRGH (0x0004) 
#define BRATE           138 // see BRG equation to see how we got here

void initUART(void);
int UARTavailable(void);
void getMessage(char *message, int maxLength);
void print(const char *string);
void println(const char *string);
void checkSerialErrors(void);

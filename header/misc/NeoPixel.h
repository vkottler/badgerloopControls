#include <xc.h>
#include "myTimer.h"
#include <stdint.h>

#define NEOPIN          LATAbits.LATA4      // pin 72
#define NUMPIXELS       16
#define NS300           19                  // 19
#define NS600           38                  // 38
#define NS900           57                  // 57

void NeoSendOne(void);
void NeoSendZero(void);
void NeoSendByte(uint8_t byte);
void NeoSend3Bytes(uint8_t *bytes);
void NeoReset(void);
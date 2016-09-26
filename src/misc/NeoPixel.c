#include "NeoPixel.h"

int i = 0;

void NeoSendOne(void) {
    NEOPIN = 1;
    TMR1 = 0; while(TMR1 < NS600);
    NEOPIN = 0;
    TMR1 = 0; while(TMR1 < NS600);
}

void NeoSendZero(void) {
    NEOPIN = 1;
    TMR1 = 0; while(TMR1 < NS300);
    NEOPIN = 0;
    TMR1 = 0; while(TMR1 < NS900);
}

void NeoSendByte(uint8_t byte) {
    for (i = 0; i < 8; i++) {
        if (byte & (1 << i)) NeoSendOne();
        else NeoSendZero();
    }
}

void NeoSend3Bytes(uint8_t *bytes) {
    for (i = 0; i < 3; i++) NeoSendByte(*(bytes+i));
}

void NeoReset(void) {
    delay1us(80);
}
    
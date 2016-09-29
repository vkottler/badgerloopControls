#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#define BOARD_LED1      LATAbits.LATA3
#define BOARD_LED2      LATCbits.LATC1
#define BOARD_LED1DIR   TRISAbits.TRISA3
#define BOARD_LED2DIR   TRISCbits.TRISC1
#define OUTPUT          0
#define INPUT           1
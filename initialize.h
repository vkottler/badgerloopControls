#include <xc.h>

// (RA3 = LD4, RC1 = LD5)
#define BOARD_LED1DIR   TRISAbits.TRISA3
#define BOARD_LED2DIR   TRISCbits.TRISC1
#define OUTPUT          0

#define BOARD_LED1      LATAbits.LATA3
#define BOARD_LED2      LATCbits.LATC1

void initializers(void);
void blinkBoardLights(int times, int time);
void setBoardLight(int light, int state);

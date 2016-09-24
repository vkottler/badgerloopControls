#include <xc.h>

void initializeTimer1(int con, int pr) {
    T1CON = con; PR1 = pr; TMR1 = 0;
}

void delay(int length, int interval) {
    int i;
    for (i = 0; i < length; i++) {
        TMR1 = 0;
        while (TMR1 < interval);
    }
}

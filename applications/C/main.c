#include "../include/main.h"

int main(void) {
    DDPCONbits.JTAGEN = 0;
    //vacuumTest();
    i2cTesting();
    return 0;
}

#include "../include/main.h"

int main(void) {
    DDPCONbits.JTAGEN = 0;
    vacuumTest();
    return 0;
}

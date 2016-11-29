#include "../include/SD.h"

int SD_init(void) {
    // set MOSI and CS to 1
    // toggle CLK for at least 74 cycles
    // set CS low
    // send CMD0: 01 000000 00000000 00000000 00000000 00000000 1001010 1
    // should get back 0 0000001
}

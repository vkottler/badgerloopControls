#include <xc.h>
#include <stdint.h>
#include "I2C.h"

// INT0 is pin 3
// INT1 is pin 2***
// INT2 is pin 7
// INT3 is pin 21
// INT4 is pin 20

#define COMPASS_ADDR 0x1E

int initCompass(void);
bool checkDataReady(void);
int CompassGetData(uint8_t *array);

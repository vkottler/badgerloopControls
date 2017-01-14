#include "globals.h"

volatile int events = 0;
int SID = 0;
uint8_t from_ID = 0;
STATE state = INIT, next_state = INIT;

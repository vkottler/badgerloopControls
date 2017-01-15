#include "globals.h"

volatile int events = 0;
int SID = 0;
uint8_t from_ID = 0;

#ifdef PRODUCTION_TESTING
    STATE state = UNDER_TEST, next_state = UNDER_TEST;
#else
    STATE state = INIT, next_state = INIT;
#endif

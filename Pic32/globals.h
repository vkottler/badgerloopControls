#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#define TIMER_5_BIT     0x1
#define TIMER_5_MASK    0xFFFFFFFE

typedef enum { INIT, IDLE, MANUAL, AUTO, BRAKE, SHUTDOWN } STATE;

typedef enum { INIT_BRAKE, ONE_AXLE_BRAKE, NORMAL_BRAKE, MAX_BRAKE } BRAKING_STATE;

typedef enum { HEALTHY, MAC_NOT_FOUND, HP_NO_RESPONSE,
               SWITCH_X_OPEN, PRESSURE_OOR, VL_NO_RESPONSE } INIT_ERROR;

extern volatile int events;
extern int SID;
extern STATE state, next_state;

#endif

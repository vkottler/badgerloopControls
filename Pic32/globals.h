#ifndef _GLOBALS__H__
#define _GLOBALS__H__

#define TIMER_5_BIT     0x1
#define TIMER_5_MASK    0xFFFFFFFE

typedef enum STATE {
    INIT,
    IDLE,
    MANUAL,
    AUTO,
    BRAKE,
    SHUTDOWN
};

typedef enum BRAKING_STATE {
    INIT,
    ONE_AXLE,
    NORMAL,
    MAX
};

typedef enum INIT_ERROR {
    HEALTHY,
    MAC_NOT_FOUND,
    HP_NO_RESPONSE,
    SWITCH_X_OPEN,
    PRESSURE_OOR,
    VL_NO_RESPONSE
};

extern volatile int events;
extern int SID;
extern STATE state, next_state;

#endif

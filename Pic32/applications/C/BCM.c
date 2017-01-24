#include "../include/BCM.h"

bool toldToInflate = false;
bool brakingReady = false;

uint8_t b1Intensity = 0, prev_b1Intensity = 0, 
        b2Intensity = 0, prev_b2Intensity = 0,
        b3Intensity = 0, prev_b3Intensity = 0,
        b4Intensity = 0, prev_b4Intensity = 0;

uint16_t B1_rpm = 0, B2_rpm = 0, B3_rpm = 0, B4_rpm = 0;

uint8_t pressure1 = 0, pressure2 = 0;

/******************************************************************************/
/*                                     Utility                                */
/******************************************************************************/
inline void inflate(void) {
    digitalWrite(MT_VALVE, 1);
    digitalWrite(VALVES, 1);
    toldToInflate = true;
    airss = PURGE_OPEN;
    if (state == DASH_CTL) next_state = READY_FOR_LAUNCH;
}

inline void b1Brake() { PWM_set_period(B1_OC, b1Intensity); }
inline void b2Brake() { PWM_set_period(B2_OC, b2Intensity); }
inline void b3Brake() { PWM_set_period(B3_OC, b3Intensity); }
inline void b4Brake() { PWM_set_period(B4_OC, b4Intensity); }

void parseBAmessage(void) {
    prev_b1Intensity = b1Intensity;
    prev_b2Intensity = b2Intensity;
    prev_b3Intensity = b3Intensity;
    prev_b4Intensity = b4Intensity;
    b1Intensity = receiving.byte0;
    b2Intensity = receiving.byte1;
    b3Intensity = receiving.byte2;
    b4Intensity = receiving.byte3;
    if (b1Intensity != prev_b1Intensity) b1Brake();
    if (b2Intensity != prev_b2Intensity) b2Brake();
    if (b3Intensity != prev_b3Intensity) b3Brake();
    if (b4Intensity != prev_b4Intensity) b4Brake();
}

bool sendBrakeState(uint16_t SID) {
    (SID & ALL) ? setupBroadcast() : setupMessage(SID);
    sending->message_num = BCM_BRAKE_STATE;
    sending->SIZE = 8;
    sending->byte0 = b1Intensity;
    sending->byte1 = b2Intensity;
    sending->byte2 = b3Intensity;
    sending->byte3 = b4Intensity;
    sending->byte4 = airss;
    sending->byte5 = pressure1;
    sending->byte6 = pressure2;
    return (SID & ALL) ? CAN_broadcast() : CAN_send(); 
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
inline void BCM_init_funcHandlers(void) {
    broadcastHandler =      &BCM_broadcast_handler;
    messageHandler =        &BCM_message_handler;

    dataProcessHandler =    &BCM_data_process_handler;

    // Main States
    dashctlHandler =        &BCM_dashctlHandler;
    rflHandler =            &BCM_rflHandler;
    pushphaseHandler =      &BCM_pushphaseHandler;
    coastHandler =          &BCM_coastHandler;
    nbrakeHandler =         &BCM_nbHandler;
    ebrakeHandler =         &BCM_ebHandler;
    fabHandler =            &BCM_fabHandler;
    rabHandler =            &BCM_rabHandler;
    wfsHandler =            &BCM_wfsHandler;
    safeHandler =           &BCM_safeHandler;
}

bool BCM_init_periph(void) {
    BCM_init_funcHandlers();
    
    // Box 1 I/O
    pinMode(X1_NC_B1, OUTPUT);
    pinMode(X1_NC_B2, OUTPUT);
    pinMode(X1_NE555_B1, OUTPUT);
    pinMode(X1_NE555_B2, OUTPUT);
    pinMode(X1_SLP_B1, OUTPUT);
    pinMode(X1_SLP_B2, OUTPUT);
    pinMode(X1_PWM_B1, OUTPUT);
    pinMode(X1_PWM_B2, OUTPUT);
    PWM_init(B1_OC);
    PWM_init(B2_OC);
    
    // Box 1: Turn on NC Relays, Disable Sleep, hold PWM low
    digitalWrite(X1_NC_B1, 1);
    digitalWrite(X1_NC_B2, 1);
    digitalWrite(X1_SLP_B1, 1);
    digitalWrite(X1_SLP_B2, 1);
    digitalWrite(X1_PWM_B1, 0);
    digitalWrite(X1_PWM_B2, 0);
    
    // Box 2 I/O
    pinMode(X2_NC_B3, OUTPUT);
    pinMode(X2_NC_B4, OUTPUT);
    pinMode(X2_NE555_B3, OUTPUT);
    pinMode(X2_NE555_B4, OUTPUT);
    pinMode(X2_SLP_B3, OUTPUT);
    pinMode(X2_SLP_B4, OUTPUT);
    pinMode(X2_PWM_B3, OUTPUT);
    pinMode(X2_PWM_B4, OUTPUT);
    PWM_init(B3_OC);
    PWM_init(B4_OC);
    
    // Box 2: Turn on NC Relays, Disable Sleep, hold PWM low
    digitalWrite(X2_NC_B3, 1);
    digitalWrite(X2_NC_B4, 1);
    digitalWrite(X2_SLP_B3, 1);
    digitalWrite(X2_SLP_B4, 1);
    digitalWrite(X2_PWM_B3, 0);
    digitalWrite(X2_PWM_B4, 0);
    
    // Valve I/0
    pinMode(MT_VALVE, OUTPUT);
    pinMode(VALVES, OUTPUT);
    
    // Valve Initialization
    digitalWrite(MT_VALVE, 0);
    digitalWrite(VALVES, 0);
    
    // Braking wheel retros
    inputCapInit(B1_IC, BCM_READINGS);
    inputCapInit(B2_IC, BCM_READINGS);
    inputCapInit(B3_IC, BCM_READINGS);
    inputCapInit(B4_IC, BCM_READINGS);
    
    return true;
}

bool BCM_broadcast_handler(void) {
    switch (receiving.message_num) {
        case DASH_BCM_AIRACTUATE:   inflate();          break;
        case DASH_BCM_BRAKEACTUATE: parseBAmessage();   break;
        case DASH_BCM_ABS_STATE:                        break;
    }
    return true;
}

bool BCM_message_handler(void) {
    switch (receiving.message_num) {
        case DASH_BCM_AIRACTUATE:   inflate();          break;
        case DASH_BCM_BRAKEACTUATE: parseBAmessage();   break;
        case DASH_BCM_ABS_STATE:                        break;
    }
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
void BCM_compute_wheel_rpms(void) {
    B1_rpm = IC3_rpm();
    B2_rpm = IC4_rpm();
    B3_rpm = IC1_rpm();
    B4_rpm = IC2_rpm();
}

void BCM_data_process_handler(void) {
    if (timer45Event) {
        sendBrakeState(ALL);
        timer45Event = false;
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
void BCM_dashctlHandler(void) {

}

// The only way to get here is by receiving INFLATE message
void BCM_rflHandler(void) {
    if (!brakingReady) {
        
        brakingReady = true;
    }
}

void BCM_pushphaseHandler(void) {
    
}

void BCM_coastHandler(void) {
    
}

void BCM_nbHandler(void) {

}

void BCM_ebHandler(void) {

}

void BCM_fabHandler(void) {

}

void BCM_rabHandler(void) {

}

void BCM_wfsHandler(void) {

}

void BCM_safeHandler(void) {
    greenOn();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
void BCM_printVariables(void) {
    printf("=================================================\r\n");
    printf("Air System State: %s\r\n", airStr[airss]);
    printf("p_b1: %3d p_b2: %3d p_b3: %3d p_b4: %3d\r\n", prev_b1Intensity, prev_b2Intensity, prev_b3Intensity, prev_b4Intensity);
    printf("  b1: %3d   b2: %3d   b3: %3d   b4: %3d\r\n", b1Intensity, b2Intensity, b3Intensity, b4Intensity);
    printf("=================================================\r\n");
}
/******************************************************************************/
/******************************************************************************/

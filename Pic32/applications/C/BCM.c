#include "../include/BCM.h"

/*
 * Overview:
 * 
 * Energizing 
 */

bool toldToInflate = false;


/******************************************************************************/
/*                                     Utility                                */
/******************************************************************************/
inline void inflate(void) {
    // Main purge HIGH
    // Deflations HIGH
    toldToInflate = true;
    airss = PURGE_OPEN;
    if (state == DASH_CTL) next_state = READY_FOR_LAUNCH;
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
    faultHandler =          &BCM_faultHandler;
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
    RD1_IN1_DIR = OUTPUT;
    RD1_IN2_DIR = OUTPUT;
    RD1_IN3_DIR = OUTPUT;
    RD1_IN4_DIR = OUTPUT;
    RD1_IN5_DIR = OUTPUT;
    RD1_IN6_DIR = OUTPUT;
    RD1_IN7_DIR = OUTPUT;
    RD1_IN1 = 0;
    RD1_IN2 = 0;
    RD1_IN3 = 0;
    RD1_IN4 = 0;
    RD1_IN5 = 0;
    RD1_IN6 = 0;
    RD1_IN7 = 0;
    
    // TODO
    // NC Relays High
    // PWM Drivers deactivated
    // 555 Timer Lines High
    // Main purge LOW
    // Deflations LOW

    return true;
}

bool BCM_broadcast_handler(void) {
    switch (receiving.message_num) {

        case DASH_BCM_AIRACTUATE: inflate(); break;

        case DASH_BCM_BRAKEACTUATE: break;

        case DASH_BCM_ABS_STATE: break;
    }
    return true;
}

bool BCM_message_handler(void) {
    switch (receiving.message_num) {
        
        case DASH_BCM_AIRACTUATE: inflate(); break;

        case DASH_BCM_BRAKEACTUATE: break;

        case DASH_BCM_ABS_STATE: break;
    }
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
void BCM_data_process_handler(void) {
    
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
void BCM_faultHandler(void) {
    redOn();
}

void BCM_dashctlHandler(void) {
    
    
}

void BCM_rflHandler(void) {
    // Main Purge:          High
    // Deflation Valves:    High
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
    
}
/******************************************************************************/
/******************************************************************************/

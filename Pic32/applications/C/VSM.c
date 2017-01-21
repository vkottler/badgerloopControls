#include "../include/VSM.h"

/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
inline void VSM_init_funcHandlers(void) {
    broadcastHandler =      &VSM_broadcast_handler;
    messageHandler =        &VSM_message_handler;

    dataProcessHandler =    &VSM_data_process_handler;

    // Main States
    faultHandler =          &VSM_faultHandler;
    dashctlHandler =        &VSM_dashctlHandler;
    rflHandler =            &VSM_rflHandler;
    pushphaseHandler =      &VSM_pushphaseHandler;
    coastHandler =          &VSM_coastHandler;
    nbrakeHandler =         &VSM_nbHandler;
    ebrakeHandler =         &VSM_ebHandler;
    fabHandler =            &VSM_fabHandler;
    rabHandler =            &VSM_rabHandler;
    wfsHandler =            &VSM_wfsHandler;
    safeHandler =           &VSM_safeHandler;
}

bool VSM_init_periph(void) {
    VSM_init_funcHandlers();
    return true;
}

bool VSM_broadcast_handler(void) {

    return true;  
}

bool VSM_message_handler(void) {

    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
void VSM_data_process_handler(void) {
    
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
void VSM_faultHandler(void) {
    redOn();
}

void VSM_dashctlHandler(void) {
    
    
}

void VSM_rflHandler(void) {
    
    
}

void VSM_pushphaseHandler(void) {
    
}

void VSM_coastHandler(void) {
    
}

void VSM_nbHandler(void) {

}

void VSM_ebHandler(void) {

}

void VSM_fabHandler(void) {

}

void VSM_rabHandler(void) {

}

void VSM_wfsHandler(void) {

}

void VSM_safeHandler(void) {
    greenOn();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
void VSM_printVariables(void) {
    printf("TODO\r\n");
}
/******************************************************************************/
/******************************************************************************/

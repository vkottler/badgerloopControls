#include "../include/VSM.h"

/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
bool VSM_init_periph(void) {
    return true;
}

bool VSM_broadcast_handler(void) {
    if (receiving.from == WCM) CAN_ping(WCM, false);
    return true;  
}

bool VSM_message_handler(void) {
    if (receiving.message_num == PING_TO) CAN_ping(receiving.from, false);
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

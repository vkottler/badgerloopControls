#include "../include/VNM.h"

/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
bool VNM_init_periph(void) {
    return true;
}

bool VNM_broadcast_handler(void) {
    if (receiving.from == WCM) CAN_ping(WCM, false);
    return true;   
}

bool VNM_message_handler(void) {
    if (receiving.message_num == PING_TO) CAN_ping(receiving.from, false);
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
void VNM_data_process_handler(void) {
    
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
void VNM_faultHandler(void) {
    redOn();
}

void VNM_dashctlHandler(void) {
    
    
}

void VNM_rflHandler(void) {
    
    
}

void VNM_pushphaseHandler(void) {
    
}

void VNM_coastHandler(void) {
    
}

void VNM_nbHandler(void) {

}

void VNM_ebHandler(void) {

}

void VNM_fabHandler(void) {

}

void VNM_rabHandler(void) {

}

void VNM_wfsHandler(void) {

}

void VNM_safeHandler(void) {
    greenOn();
}
/******************************************************************************/
/******************************************************************************/

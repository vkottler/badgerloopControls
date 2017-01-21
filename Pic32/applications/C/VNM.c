#include "../include/VNM.h"

volatile bool VNM_getMPU = false;
COORD_VECTOR accelData;

uint16_t px = 0, py = 0, pz = 0, vx = 0, vy = 0, vz = 0, 
         ax = 0, ay = 0, az = 0;

/******************************************************************************/
/*                             Outgoing Messages                              */
/******************************************************************************/
bool VNM_sendPos(void) {
    setupBroadcast();
    sending->SIZE = 7;
    sending->byte0 = px >> 8;
    sending->byte1 = px & 0xff;
    sending->byte2 = py >> 8;
    sending->byte3 = py & 0xff;
    sending->byte4 = pz >> 8;
    sending->byte5 = pz & 0xff;
}

bool VNM_sendVel(void) {
    setupBroadcast();
    sending->SIZE = 7;
    sending->byte0 = vx >> 8;
    sending->byte1 = vx & 0xff;
    sending->byte2 = vy >> 8;
    sending->byte3 = vy & 0xff;
    sending->byte4 = vz >> 8;
    sending->byte5 = vz & 0xff;
}

bool VNM_sendAcc(void) {
    setupMessage(WCM);
    sending->SIZE = 7;
    sending->byte0 = ax >> 8;
    sending->byte1 = ax & 0xff;
    sending->byte2 = ay >> 8;
    sending->byte3 = ay & 0xff;
    sending->byte4 = az >> 8;
    sending->byte5 = az & 0xff;
}

bool VNM_sendAtt(void) {
    setupMessage(WCM);
    sending->SIZE = 7;
    // pitch
    // roll
    // yaw
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
bool VNM_init_periph(void) {
    I2Cinit();
    memset(&accelData, 0, sizeof(COORD_VECTOR));
    initializeSlowTimer(MPU_SAMPLE_PERIOD);
    // TODO
    // Initialize input capture
    return MPUinitialize();
}

bool VNM_broadcast_handler(void) {
    
    return true;   
}

bool VNM_message_handler(void) {
    
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
void VNM_data_process_handler(void) {
    if (VNM_getMPU) {
        MPUread(&accelData);
        VNM_getMPU = false;
    }
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

#include "../include/VNM.h"

volatile bool VNM_getMPU = false;
COORD_VECTOR accelData;

volatile bool *front_rdy, *middle_rdy, *rear_rdy;

volatile unsigned int *frontInterval, *middleInterval, *rearInterval;

unsigned int frontVelocity = 0, middleVelocity = 0, rearVelocity = 0;

uint16_t px = 0, py = 0, pz = 0, vx = 0, vy = 0, vz = 0, 
         ax = 0, ay = 0, az = 0;

uint16_t frontCount = 0, middleCount = 0, rearCount = 0;
uint8_t frontFaults = 0, rearFaults = 0, middleFaults = 0;

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
inline void VNM_init_funcHandlers(void) {
    broadcastHandler =      &VNM_broadcast_handler;
    messageHandler =        &VNM_message_handler;

    dataProcessHandler =    &VNM_data_process_handler;

    // Main States
    faultHandler =          &VNM_faultHandler;
    dashctlHandler =        &VNM_dashctlHandler;
    rflHandler =            &VNM_rflHandler;
    pushphaseHandler =      &VNM_pushphaseHandler;
    coastHandler =          &VNM_coastHandler;
    nbrakeHandler =         &VNM_nbHandler;
    ebrakeHandler =         &VNM_ebHandler;
    fabHandler =            &VNM_fabHandler;
    rabHandler =            &VNM_rabHandler;
    wfsHandler =            &VNM_wfsHandler;
    safeHandler =           &VNM_safeHandler;
}

bool VNM_init_periph(void) {
    VNM_init_funcHandlers();
    I2Cinit();
    memset(&accelData, 0, sizeof(COORD_VECTOR));
    initializeSlowTimer(MPU_SAMPLE_PERIOD);
    inputCapInit(1, 1);
    inputCapInit(4, 1);
    inputCapInit(5, 1);
    front_rdy = &IC1ready;
    middle_rdy = &IC4ready;
    rear_rdy = &IC5ready;
    frontInterval= IC1times;
    middleInterval = IC4times;
    rearInterval= IC5times;
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
    if (FRONT_MISS) frontFaults++;
    if (MIDDLE_MISS) middleFaults++;
    if (REAR_MISS) rearFaults++;
    if (*front_rdy) {
        frontVelocity = getFrequency(*frontInterval) * 30;
        frontCount++;
    }
    if (*middle_rdy) {
        middleVelocity = getFrequency(*middleInterval) * 30;
        middleCount++;
    }
    if (*rear_rdy) {
        rearVelocity = getFrequency(*rearInterval) * 30;
        rearCount++;
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


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
//uint16_t px = 0, py = 0, pz = 0, vx = 0, vy = 0, vz = 0, 
//         ax = 0, ay = 0, az = 0;

void VNM_printVariables(void) {
    printf("=================================================\r\n");
    printf("ax:%5d ay:%5d az:%5d ", accelData.ax, accelData.ay, accelData.az);
    printf("gx:%5d gy:%5d gz:%5d\r\n", accelData.gx, accelData.gy, accelData.gz);
    printf("px:%5d py:%5d pz:%5d vx:%5d vy:%5d vz:%5d\r\n", px, py, pz, vx, vy, vz);
    printf("ax:%5d ay:%5d az:%5d\r\n", ax, ay, az);
    printf("=================================================\r\n");
    printf("strips ([F][M][R]):\t[%3d][%3d][%3d]\r\n", frontCount, middleCount, rearCount);
    printf("velocity (m/s):\t\t[%4d][%4d][%4d]\r\n", frontVelocity, middleVelocity, rearVelocity);
    printf("=================================================\r\n");
}
/******************************************************************************/
/******************************************************************************/

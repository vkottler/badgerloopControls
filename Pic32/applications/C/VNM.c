#include "../include/VNM.h"

COORD_VECTOR accelData;

unsigned int frontVelocity = 0, middleVelocity = 0, rearVelocity = 0;

STRIP_SEQUENCE curr_strip = FRONT;

volatile int *main_count = NULL;

uint16_t px = 0, py = 0, pz = 0, 
         vx = 0, vy = 0, vz = 0, 
         ax = 0, ay = 0, az = 0;

uint8_t frontFaults = 0, rearFaults = 0, middleFaults = 0;

/******************************************************************************/
/*                             Outgoing Messages                              */
/******************************************************************************/
bool VNM_sendPos(void) {
    setupBroadcast();
    sending->SIZE = 7;
    sending->message_num = VNM_POS;
    sending->byte0 = px >> 8;
    sending->byte1 = px & 0xff;
    sending->byte2 = py >> 8;
    sending->byte3 = py & 0xff;
    sending->byte4 = pz >> 8;
    sending->byte5 = pz & 0xff;
    return CAN_broadcast();
}

bool VNM_sendVel(void) {
    setupBroadcast();
    sending->SIZE = 7;
    sending->message_num = VNM_VEL;
    sending->byte0 = vx >> 8;
    sending->byte1 = vx & 0xff;
    sending->byte2 = vy >> 8;
    sending->byte3 = vy & 0xff;
    sending->byte4 = vz >> 8;
    sending->byte5 = vz & 0xff;
    return CAN_broadcast();
}

bool VNM_sendAcc(void) {
    setupBroadcast();
    sending->message_num = VNM_ACC;
    sending->SIZE = 7;
    sending->byte0 = ax >> 8;
    sending->byte1 = ax & 0xff;
    sending->byte2 = ay >> 8;
    sending->byte3 = ay & 0xff;
    sending->byte4 = az >> 8;
    sending->byte5 = az & 0xff;
    return CAN_broadcast();
}

bool VNM_sendAtt(void) {
    setupBroadcast();
    sending->SIZE = 7;
    // pitch
    // roll
    // yaw
}

bool VNM_sendStrip(void) {
    setupBroadcast();
    sending->message_num = VNM_STRIP_COUNT;
    sending->SIZE = 7;
    sending->byte0 = FRONT_COUNT >> 8;
    sending->byte1 = FRONT_COUNT & 0xff;
    sending->byte2 = MIDDLE_COUNT >> 8;
    sending->byte3 = MIDDLE_COUNT & 0xff;
    sending->byte4 = REAR_COUNT >> 8;
    sending->byte5 = REAR_COUNT & 0xff;
    return CAN_broadcast();
}

bool VNM_sendLost(void) {
    setupBroadcast();
    sending->message_num = VNM_STRIPLOST;
    sending->SIZE = 4;
    sending->byte0 = frontFaults;
    sending->byte1 = middleFaults;
    sending->byte2 = rearFaults;
    return CAN_broadcast();
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
    CANsendHandler =        &VNM_CANsendHandler;

    // Main States
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
    main_count = &FRONT_COUNT;
    memset(&accelData, 0, sizeof(COORD_VECTOR));
    inputCapInit(1);
    inputCapInit(4);
    inputCapInit(5);
    
    digitalWrite(50, 0);
    digitalWrite(51, 0);
    
    //inputCapInit(2);
    //inputCapInit(3);
    
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
    MPUread(&accelData);
    if (FRONT_MISS) {
        VNM_sendLost();
        frontFaults++;
    }
    if (MIDDLE_MISS) {
        VNM_sendLost();
        middleFaults++;
    }
    if (REAR_MISS) {
        VNM_sendLost();
        rearFaults++;
    }
    
    
    
    
    
    if (timer45Event) {
        
        timer45Event = false;
    }
}

void VNM_CANsendHandler(void) {
    VNM_sendAcc();
    VNM_sendVel();
    VNM_sendStrip();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
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
    printf("strips ([F][M][R]):\t[%3d][%3d][%3d]\r\n", FRONT_COUNT, MIDDLE_COUNT, REAR_COUNT);
    printf("velocity (m/s):\t\t[%4d][%4d][%4d]\r\n", frontVelocity, middleVelocity, rearVelocity);
    printf("=================================================\r\n");
}
/******************************************************************************/
/******************************************************************************/

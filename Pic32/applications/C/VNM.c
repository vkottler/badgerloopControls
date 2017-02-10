#include "../include/VNM.h"

uint8_t vectorIndex = 0;

COORD_VECTOR mpuVec;

uint16_t px = 0, py = 0, pz = 0;
uint16_t frontVelocity = 0, middleVelocity = 0, rearVelocity = 0;
float ax = 0.0, ay = 0.0, az = 0.0; 

volatile unsigned int *main_count = NULL;

uint8_t frontFaults = 0, rearFaults = 0, middleFaults = 0;

bool firstStripSent = false;


inline void calculateVelocity(void) {
    __builtin_disable_interrupts();
    frontVelocity = getFrequency(FRONT_INTERVAL);
    frontVelocity = frontVelocity * 30 + frontVelocity / 2;
    middleVelocity = getFrequency(MIDDLE_INTERVAL);
    middleVelocity = frontVelocity * 30 + frontVelocity / 2;
    rearVelocity = getFrequency(REAR_INTERVAL);
    rearVelocity = frontVelocity * 30 + frontVelocity / 2;
    __builtin_enable_interrupts();    
}


/******************************************************************************/
/*                             Outgoing Messages                              */
/******************************************************************************/
void VNM_sendPos(void) {
    px = *main_count * 30 + *main_count / 2;
    setupBroadcast();
    sending->SIZE = 7;
    sending->message_num = VNM_POS;
    sending->byte0 = px >> 8;
    sending->byte1 = px & 0xff;
    sending->byte2 = py >> 8;
    sending->byte3 = py & 0xff;
    sending->byte4 = pz >> 8;
    sending->byte5 = pz & 0xff;
    handleCANbco();
}

void VNM_sendVel(void) {
    calculateVelocity();
    setupBroadcast();
    sending->SIZE = 7;
    sending->message_num = VNM_VEL;
    sending->byte0 = frontVelocity >> 8;
    sending->byte1 = frontVelocity & 0xff;
    sending->byte2 = middleVelocity >> 8;
    sending->byte3 = middleVelocity & 0xff;
    sending->byte4 = rearVelocity >> 8;
    sending->byte5 = rearVelocity & 0xff;
    handleCANbco();
}

/*
void VNM_sendAcc(void) {
    setupBroadcast();
    sending->message_num = VNM_ACC;
    sending->SIZE = 7;
    sending->byte0 = ax >> 8;
    sending->byte1 = ax & 0xff;
    sending->byte2 = ay >> 8;
    sending->byte3 = ay & 0xff;
    sending->byte4 = az >> 8;
    sending->byte5 = az & 0xff;
    handleCANbco();
}
*/

void VNM_sendAtt(void) {
    setupBroadcast();
    sending->SIZE = 7;
    // pitch
    // roll
    // yaw
}

void VNM_sendStrip(void) {
    setupBroadcast();
    sending->message_num = VNM_STRIP_COUNT;
    sending->SIZE = 4;
    sending->byte0 = *main_count >> 8;
    sending->byte1 = *main_count & 0xff;
    if (main_count == &FRONT_COUNT) sending->byte2 = 0;
    else if (main_count == &MIDDLE_COUNT) sending->byte2 = 1;
    else sending->byte2 = 2;
    handleCANbco();
}

void VNM_sendLost(void) {
    setupBroadcast();
    sending->message_num = VNM_STRIPLOST;
    sending->SIZE = 4;
    sending->byte0 = frontFaults;
    sending->byte1 = middleFaults;
    sending->byte2 = rearFaults;
    handleCANbco();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/


inline void handleStripDetection(void) {
    if (main_count == &FRONT_COUNT) {
        if (FRONT_COUNT != MIDDLE_COUNT && MIDDLE_COUNT == REAR_COUNT && MIDDLE_COUNT > FRONT_COUNT)
            main_count = &MIDDLE_COUNT;
    }
    else if (main_count == &MIDDLE_COUNT) {
        if (MIDDLE_COUNT < REAR_COUNT)
            main_count = &REAR_COUNT;
    }
    else {
        if (FRONT_COUNT > REAR_COUNT && FRONT_COUNT > MIDDLE_COUNT)
            main_count = &FRONT_COUNT;
        else if (MIDDLE_COUNT > REAR_COUNT && MIDDLE_COUNT > FRONT_COUNT)
            main_count = &MIDDLE_COUNT;
    }
}

inline void MPU_data_process(void) {
    /*
    __builtin_disable_interrupts();
    mpuVec.ax = (int16_t) (((int16_t)mpuBytes[0] << 8) | mpuBytes[1]  );
    mpuVec.fax = (float) mpuVec.ax / ACCEL_SCALAR;
    mpuVec.ay = (int16_t) (((int16_t)mpuBytes[2] << 8) | mpuBytes[3]  );
    mpuVec.fay = (float) mpuVec.ay / ACCEL_SCALAR;
    mpuVec.az = (int16_t) (((int16_t)mpuBytes[4] << 8) | mpuBytes[5]  );
    mpuVec.faz = (float) mpuVec.az / ACCEL_SCALAR;
    mpuVec.gx = (int16_t) (((int16_t)mpuBytes[8] << 8) | mpuBytes[9]  );
    mpuVec.fgx = (float) mpuVec.gx / GYRO_SCALAR;
    mpuVec.gy = (int16_t) (((int16_t)mpuBytes[10] << 8) | mpuBytes[11]  );
    mpuVec.fgy = (float) mpuVec.gy / GYRO_SCALAR;
    mpuVec.gz = (int16_t) (((int16_t)mpuBytes[12] << 8) | mpuBytes[13]  );
    mpuVec.fgz = (float) mpuVec.gz / GYRO_SCALAR;
    __builtin_enable_interrupts();
    */    
}

inline void checkFirstStrip(void) {
    if (*main_count && !firstStripSent) {
        setupBroadcast();
        sending->message_num = ENTER_STATE;
        sending->SIZE = 2;
        sending->byte0 = COAST;
        handleCANbco();
        firstStripSent = true;
    }       
}

inline void handleMPU(void) {
    if (MPU_ready) {
        MPU_data_process();
        MPU_ready = false;
    }
    else MPU_step();    
}

void VNM_data_process_handler(void) {
    
    // Check sensor integrity
    handleStripDetection();
    
    // Check accelerometer
    //handleMPU();
    
    // Check software missed strips
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
    
    // Check interval
    if (timer45Event) {
        
        timer45Event = false;
    }
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
    memset((void *) &mpuVec, 0, sizeof(COORD_VECTOR));
    VNM_init_funcHandlers();
    I2Cinit();
    main_count = &FRONT_COUNT;
    inputCapInit(1);
    inputCapInit(4);
    inputCapInit(5);
    return MPUinitialize();
}

bool VNM_broadcast_handler(void) {
    
    return true;   
}

bool VNM_message_handler(void) {
    
    return true;
}

void VNM_CANsendHandler(void) {
    VNM_sendPos();
    //VNM_sendAcc();
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
    if (*main_count && !firstStripSent) {
        setupBroadcast();
        sending->message_num = ENTER_STATE;
        sending->SIZE = 2;
        sending->byte0 = COAST;
        handleCANbco();
        firstStripSent = true;
    }      
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
    
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
void VNM_printVariables(void) {
    //printf("=================================================\r\n");
    printf("ax:%.5d ay:%.5d az:%.5d ", mpuVec.ax, mpuVec.ay, mpuVec.az);
    printf("gx:%.5d gy:%.5d gz:%.5d\r\n", mpuVec.gx, mpuVec.gy, mpuVec.gz);
    printf("ax:%.5f ay:%.5f az:%.5f ", mpuVec.fax, mpuVec.fay, mpuVec.faz);
    printf("gx:%.5f gy:%.5f gz:%.5f\r\n", mpuVec.fgx, mpuVec.fgy, mpuVec.fgz);
    //printf("px:%5d py:%5d pz:%5d vx:%5d vy:%5d vz:%5d\r\n", px, py, pz, vx, vy, vz);
    //printf("ax:%5d ay:%5d az:%5d\r\n", ax, ay, az);
    //printf("=================================================\r\n");
    //printf("strips ([F][M][R]):\t[%3d][%3d][%3d]\r\n", FRONT_COUNT, MIDDLE_COUNT, REAR_COUNT);
    //printf("velocity (m/s):\t\t[%4d][%4d][%4d]\r\n", frontVelocity, middleVelocity, rearVelocity);
    //printf("=================================================\r\n");
}
/******************************************************************************/
/******************************************************************************/

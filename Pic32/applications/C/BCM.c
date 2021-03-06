#include "../include/BCM.h"

bool toldToInflate = false;
bool brakingReady = false;
bool eBrakeOn = false;

uint8_t intensities[] =         {0, 0, 0, 0, 0};
uint8_t prev_intensities[] =    {0, 0, 0, 0, 0};

uint16_t B1_rpm = 0, B2_rpm = 0, B3_rpm = 0, B4_rpm = 0;
uint16_t B1_fwv = 0, B2_fwv = 0, B3_fwv = 0, B4_fwv = 0;

uint16_t pressure1 = 0, pressure2 = 0;

uint8_t currADC = MT1A;

uint16_t strip_count = 0;
uint16_t x_pos = 0;

/******************************************************************************/
/*                                     Utility                                */
/******************************************************************************/
void setBrakeIntensity(uint8_t brake, uint8_t intensity) {
    prev_intensities[brake] = intensities[brake];
    intensities[brake] = intensity;
}

void readyBrakes(void) {
    digitalWrite(X1_NE555_B1, 0);
    digitalWrite(X1_NE555_B2, 0);
    digitalWrite(X2_NE555_B3, 0);
    digitalWrite(X2_NE555_B4, 0);
    brakingReady = true;
    brakesOff();
}

void unreadyBrakes(void) {
    digitalWrite(X1_NE555_B1, 1);
    digitalWrite(X1_NE555_B2, 1);
    digitalWrite(X2_NE555_B3, 1);
    digitalWrite(X2_NE555_B4, 1);
    brakingReady = false;
    brakesOff();
}

inline void brakesOff(void) {
    setBrakeIntensity(1, 0);
    setBrakeIntensity(2, 0);
    setBrakeIntensity(3, 0);
    setBrakeIntensity(4, 0);
}

void deflate(void) {
    digitalWrite(MT_VALVE, 0);
    digitalWrite(VALVES , 0);
    airss = DEFLATED;
    if (state == READY_FOR_LAUNCH) next_state = DASH_CTL;
}

void eBrake(void) {
    digitalWrite(X1_NC_B1, 0);
    digitalWrite(X1_NC_B2, 0);
    digitalWrite(X2_NC_B3, 0);
    digitalWrite(X2_NC_B4, 0);
    setBrakeIntensity(1, 100);
    setBrakeIntensity(2, 100);
    setBrakeIntensity(3, 100);
    setBrakeIntensity(4, 100);
}

void inflate(void) {
    digitalWrite(MT_VALVE, 1);
    delay(500, MILLI);
    digitalWrite(VALVES, 1);
    toldToInflate = true;
    airss = PURGE_OPEN;
    if (state == DASH_CTL) next_state = READY_FOR_LAUNCH;
}

inline void b1Brake(void) { PWM_set_period(B1_OC, intensities[1]); }
inline void b2Brake(void) { PWM_set_period(B2_OC, intensities[2]); }
inline void b3Brake(void) { PWM_set_period(B3_OC, intensities[3]); }
inline void b4Brake(void) { PWM_set_period(B4_OC, intensities[4]); }

inline void updateBrakes(void) {
    b1Brake();
    b2Brake();
    b3Brake();
    b4Brake();
}

void parseBAmessage(void) {
    setBrakeIntensity(1, receiving.byte0);
    setBrakeIntensity(2, receiving.byte1);
    setBrakeIntensity(3, receiving.byte2);
    setBrakeIntensity(4, receiving.byte3);
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
void BCM_brake_speed(bool message1) {
    setupMessage(WCM);
    sending->message_num = message1 ? BCM_BRAKE_SPEED1 : BCM_BRAKE_SPEED2;
    sending->SIZE = 5;
    sending->byte0 = (message1 ? B1_rpm : B3_rpm) >> 8;
    sending->byte1 = (message1 ? B1_rpm : B3_rpm) & 0xff;
    sending->byte2 = (message1 ? B2_rpm : B4_rpm) >> 8;
    sending->byte3 = (message1 ? B2_rpm : B4_rpm) & 0xff;
    handleCANbco();
}

void sendBrakeState(uint16_t SID) {
    (SID & ALL) ? setupBroadcast() : setupMessage(SID);
    sending->message_num = BCM_BRAKE_STATE;
    sending->SIZE = 8;
    sending->byte0 = intensities[1];
    sending->byte1 = intensities[2];
    sending->byte2 = intensities[3];
    sending->byte3 = intensities[4];
    sending->byte4 = airss;
    sending->byte5 = pressure1;
    sending->byte6 = pressure2;
    (SID & ALL) ? handleCANbco() : handleCANmo; 
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
    CANsendHandler =        &BCM_CANsendHandler;

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
    digitalWrite(X1_NE555_B1, 1);
    digitalWrite(X1_NE555_B2, 1);
    
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
    digitalWrite(X2_NE555_B3, 1);
    digitalWrite(X2_NE555_B4, 1);
    
    // Valve I/0
    pinMode(MT_VALVE, OUTPUT);
    pinMode(VALVES, OUTPUT);
    
    // Valve Initialization
    digitalWrite(MT_VALVE, 0);
    digitalWrite(VALVES, 0);
    
    // Braking wheel retros
    inputCapInit(B1_IC);
    inputCapInit(B2_IC);
    inputCapInit(B3_IC);
    inputCapInit(B4_IC);
    
    return true;
}

bool BCM_broadcast_handler(void) {
    switch (receiving.message_num) {
        case DASH_BCM_AIRACTUATE: receiving.byte0 ? inflate() : deflate(); break;      
        case DASH_BCM_BRAKEACTUATE: parseBAmessage();       break;
        case DASH_BCM_ABS_STATE:                            break;
        case VNM_STRIP_COUNT: strip_count = GET_CAN_WORD;   break;
        case VNM_POS: x_pos = GET_CAN_WORD;                 break;
    }
    return true;
}

bool BCM_message_handler(void) {
    switch (receiving.message_num) {
        case DASH_BCM_AIRACTUATE: receiving.byte0 ? inflate() : deflate(); break;
        case DASH_BCM_BRAKEACTUATE: parseBAmessage();       break;
        case DASH_BCM_ABS_STATE:                            break;
        case VNM_STRIP_COUNT: strip_count = GET_CAN_WORD;   break;
        case VNM_POS: x_pos = GET_CAN_WORD;                 break;
    }
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
inline void BCM_handle_ADC(void) {
    if  (!sampling) {
        ADCstartSample(currADC);
        currADC = (currADC == MT1A) ? MT2A : MT1A;
    }
    
    if (READING_READY) {
        currADC == MT1A ? ADCread(&pressure2) : ADCread(&pressure1);
        currADC = (currADC == MT1A) ? MT2A : MT1A;
    }    
}

void BCM_compute_wheel_velocity(void) {
    B1_fwv = B1_rpm / RPM_DIVISOR;
    B1_fwv = B1_rpm / RPM_DIVISOR;
    B1_fwv = B1_rpm / RPM_DIVISOR;
    B1_fwv = B1_rpm / RPM_DIVISOR;
}

void BCM_compute_wheel_rpms(void) {
    B1_rpm = IC3_rpm();
    B2_rpm = IC4_rpm();
    B3_rpm = IC1_rpm();
    B4_rpm = IC2_rpm();
}

void BCM_data_process_handler(void) {
    BCM_compute_wheel_velocity();
    
    BCM_handle_ADC();
    
    if (timer45Event) {
        
        timer45Event = false;
    }
    
    // Save for ADC values
    if (false) {
        airss = INFLATED;
    }
}

void BCM_CANsendHandler(void) {
    sendBrakeState(ALL);
    BCM_compute_wheel_rpms();
    BCM_brake_speed(true);
    BCM_brake_speed(false);
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
void BCM_dashctlHandler(void) {
    if (brakingReady) unreadyBrakes();
}

// The only way to get here is by receiving INFLATE message
void BCM_rflHandler(void) {
    if (brakingReady) unreadyBrakes();
}

void BCM_pushphaseHandler(void) {
    if (!brakingReady) readyBrakes();
}

void BCM_coastHandler(void) {
    if (!brakingReady) readyBrakes();
}

void BCM_nbHandler(void) {
    if (airss == PURGE_OPEN || INFLATED) deflate();
    if (!brakingReady) readyBrakes();
    updateBrakes();
}

void BCM_ebHandler(void) {
    if (airss == PURGE_OPEN || INFLATED) deflate();
    if (!brakingReady) readyBrakes();
    if (!eBrakeOn) eBrake();
    updateBrakes();
}

void BCM_fabHandler(void) {

}

void BCM_rabHandler(void) {

}

void BCM_wfsHandler(void) {

}

void BCM_safeHandler(void) {
    brakesOff();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
void BCM_printVariables(void) {
    uint8_t i;
    printf("=================================================\r\n");
    printf("Air System State: %s\r\n", airStr[airss]);
    for ( i = 1; i < 5; i++) printf("p_b%d: %3d ", i, prev_intensities[i]);
    printf("\r\n");
    for ( i = 1; i < 5; i++) printf("  b%d: %3d ", i, intensities[i]);
    printf("\r\n");
    printf("=================================================\r\n");
}
/******************************************************************************/
/******************************************************************************/

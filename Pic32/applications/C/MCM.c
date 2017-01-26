#include "../include/MCM.h"

uint16_t left_front_rpm = 0, right_front_rpm = 0, left_rear_rpm = 0, right_rear_rpm = 0;

uint16_t commanded_speed = 0, new_speed = 0;

/******************************************************************************/
/*                                  Utility                                   */
/******************************************************************************/
void stop_wheels(void) {
    if (commanded_speed != 0) {
        mcp_write_val(0x0000);
        commanded_speed = 0;
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                             Outgoing Messages                              */
/******************************************************************************/
bool send_wheel_rpms(uint16_t SID) {
    bool result = true;
    
    (SID & ALL) ? setupBroadcast() : setupMessage(SID);
    sending->SIZE = 5;
    sending->message_num = MCM_HB_SPEED1;
    sending->byte0 = left_front_rpm >> 8;
    sending->byte1 = left_front_rpm & 0xff;
    sending->byte2 = right_front_rpm >> 8;
    sending->byte3 = right_front_rpm & 0xff;
    result = (SID & ALL) ? CAN_broadcast() : CAN_send();
    
    (SID & ALL) ? setupBroadcast() : setupMessage(SID);
    sending->SIZE = 5;
    sending->message_num = MCM_HB_SPEED2;
    sending->byte0 = left_rear_rpm >> 8;
    sending->byte1 = left_rear_rpm & 0xff;
    sending->byte2 = right_rear_rpm >> 8;
    sending->byte3 = right_rear_rpm & 0xff;
    return ((SID & ALL) ? CAN_broadcast() : CAN_send()) && result;
}

bool send_cmdv(uint16_t SID) {
    (SID & ALL) ? setupBroadcast() : setupMessage(SID);
    sending->SIZE = 3;
    sending->message_num = MCM_CMDV;
    sending->byte0 = commanded_speed >> 8;
    sending->byte1 = commanded_speed & 0xff;
    return (SID & ALL) ? CAN_broadcast() : CAN_send();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
void MCM_compute_wheel_rpms(void) {
    left_front_rpm =    IC2_rpm();
    right_front_rpm=    IC5_rpm();
    left_rear_rpm =     IC1_rpm();
    right_rear_rpm =    IC4_rpm();
}

void check_cmdv(void) {
    
}

void MCM_data_process_handler(void) {
    MCM_compute_wheel_rpms();
    check_cmdv();
    if (timer45Event) {

        timer45Event = false;
    }
}

void MCM_CANsendHandler(void) {
    send_cmdv(ALL);
    send_wheel_rpms(ALL);
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
void MCM_init_funcHandlers(void) {
    broadcastHandler =      &MCM_broadcast_handler;
    messageHandler =        &MCM_message_handler;

    dataProcessHandler =    &MCM_data_process_handler;
    CANsendHandler =        &MCM_CANsendHandler;

    // Main States
    dashctlHandler =        &MCM_dashctlHandler;
    rflHandler =            &MCM_rflHandler;
    pushphaseHandler =      &MCM_pushphaseHandler;
    coastHandler =          &MCM_coastHandler;
    nbrakeHandler =         &MCM_nbHandler;
    ebrakeHandler =         &MCM_ebHandler;
    fabHandler =            &MCM_fabHandler;
    rabHandler =            &MCM_rabHandler;
    wfsHandler =            &MCM_wfsHandler;
    safeHandler =           &MCM_safeHandler;
}

bool MCM_init_periph(void) {
    MCM_init_funcHandlers();
    I2Cinit();
    mcp_write_val(0x0000);              // do not throttle wheels
    inputCapInit(2);
    inputCapInit(5);
    inputCapInit(1);
    inputCapInit(4);
    return true;
}

bool MCM_broadcast_handler(void) {

    return true;
}

bool MCM_message_handler(void) {
    switch (receiving.message_num) {
        
        case MCM_CMDV: send_cmdv(receiving.from); break;
            
        case DASH_MCM_SPINWHEELS: 
            new_speed = (receiving.byte0) | (receiving.byte1 & 0xff);
            if (new_speed != commanded_speed) {
                mcp_write_val(new_speed);
                commanded_speed = new_speed;
            }
            break;
            
    }
    return true;
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
void MCM_dashctlHandler(void) {
    
    
}

void MCM_rflHandler(void) {
    
    
}

void MCM_pushphaseHandler(void) {
    
}

void MCM_coastHandler(void) {
    
}

void MCM_nbHandler(void) {
    stop_wheels();

}

void MCM_ebHandler(void) {
    stop_wheels();

}

void MCM_fabHandler(void) {
    stop_wheels();

}

void MCM_rabHandler(void) {
    stop_wheels();

}

void MCM_wfsHandler(void) {
    stop_wheels();

}

void MCM_safeHandler(void) {
    greenOn();

}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
void MCM_printVariables(void) {
    printf("LF: %5d RF: %5d LR: %5d RR: %5d\r\n", left_front_rpm, right_front_rpm, left_rear_rpm, right_rear_rpm);
}
/******************************************************************************/
/******************************************************************************/
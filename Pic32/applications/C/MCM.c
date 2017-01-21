#include "../include/MCM.h"

volatile bool *left_front_HB_rdy, *right_front_HB_rdy, *left_rear_HB_rdy, *right_rear_HB_rdy;

volatile unsigned int *left_front_readings, *right_front_readings, *back_left_readings, *back_right_readings;

uint16_t left_front_rpm, right_front_rpm, left_rear_rpm, right_rear_rpm;

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
bool send_wheel_rpms(void) {
    bool result = true;
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = ourRole;
    sending->SIZE = 5;
    sending->message_num = MCM_HB_SPEED1;
    sending->byte0 = left_front_rpm >> 8;
    sending->byte1 = left_front_rpm & 0xff;
    sending->byte2 = right_front_rpm >> 8;
    sending->byte3 = right_front_rpm & 0xff;
    result = CAN_broadcast();
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ALL;
    sending->from = ourRole;
    sending->SIZE = 5;
    sending->message_num = MCM_HB_SPEED2;
    sending->byte0 = left_rear_rpm >> 8;
    sending->byte1 = left_rear_rpm & 0xff;
    sending->byte2 = right_rear_rpm >> 8;
    sending->byte3 = right_rear_rpm & 0xff;
    return result && CAN_broadcast();
}

bool send_cmdv(ROLE to) {
    sending = BROADCAST_SEND_ADDR;
    sending->SID = ROLEtoSID(to);
    sending->from = ourRole;
    sending->SIZE = 3;
    sending->message_num = MCM_CMDV;
    sending->byte0 = commanded_speed >> 8;
    sending->byte1 = commanded_speed & 0xff;
    return CAN_send();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Data Processing & Unit Conversions                  */
/******************************************************************************/
void compute_wheel_rpms(void) {
    uint8_t i;
    unsigned int averageInterval = 0;
    if (*left_front_HB_rdy) {
        LEFT_FRONT_ENABLE_INT = 0;
        for (i = 0; i < WHEEL_READINGS - 1; i++) averageInterval += left_front_readings[i + 1] - left_front_readings[i];
        averageInterval = averageInterval / (WHEEL_READINGS - 1);
        left_front_rpm = getRPM(averageInterval) & 0xffff;
        *left_front_HB_rdy = false;
        LEFT_FRONT_ENABLE_INT = 1;
    }
    if (*right_front_HB_rdy) {
        RIGHT_FRONT_ENABLE_INT = 0;
        for (i = 0; i < WHEEL_READINGS - 1; i++) averageInterval += right_front_readings[i + 1] - right_front_readings[i];
        averageInterval = averageInterval / (WHEEL_READINGS - 1);
        right_front_rpm = getRPM(averageInterval) & 0xffff;
        *right_front_HB_rdy = false;
        RIGHT_FRONT_ENABLE_INT = 1;        
    }
    if (*left_rear_HB_rdy) {
        LEFT_REAR_ENABLE_INT = 0;
        for (i = 0; i < WHEEL_READINGS - 1; i++) averageInterval += left_front_readings[i + 1] - left_front_readings[i];
        averageInterval = averageInterval / (WHEEL_READINGS - 1);
        left_rear_rpm = getRPM(averageInterval) & 0xffff;
        *left_rear_HB_rdy = false;
        LEFT_REAR_ENABLE_INT = 1;        
    }
    if (*right_rear_HB_rdy) {
        RIGHT_REAR_ENABLE_INT = 0;
        for (i = 0; i < WHEEL_READINGS - 1; i++) averageInterval += left_front_readings[i + 1] - left_front_readings[i];
        averageInterval = averageInterval / (WHEEL_READINGS - 1);
        right_rear_rpm = getRPM(averageInterval) & 0xffff;
        *right_rear_HB_rdy = false;
        RIGHT_REAR_ENABLE_INT = 1;         
    }
}

void MCM_data_process_handler(void) {
    compute_wheel_rpms();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
bool MCM_init_periph(void) {
    I2Cinit();
    mcp_write_val(0x0000);              // do not throttle wheels
    inputCapInit(2, WHEEL_READINGS);
    inputCapInit(5, WHEEL_READINGS);
    inputCapInit(1, WHEEL_READINGS);
    inputCapInit(4, WHEEL_READINGS);
    left_front_HB_rdy = &IC2ready;
    right_front_HB_rdy = &IC5ready;
    left_rear_HB_rdy = &IC1ready;
    right_rear_HB_rdy = &IC4ready;
    left_front_readings = IC2times;
    right_front_readings = IC5times;
    back_left_readings = IC1times;
    back_right_readings = IC4times;
    left_front_rpm = 0;
    right_front_rpm = 0;
    left_rear_rpm = 0;
    right_rear_rpm = 0;    
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
void MCM_faultHandler(void) {
    redOn();
}

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

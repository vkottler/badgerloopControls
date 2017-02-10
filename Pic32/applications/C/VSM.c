#include "../include/VSM.h"

bool left_door_state, right_door_state, prev_rds, prev_lds;

bool doorChanged = false;

uint8_t currIndex = 1;
uint16_t readings[10];

volatile VSM_TEMPS temps;

/******************************************************************************/
/*              Initialization and Message Reception Behavior                 */
/******************************************************************************/
inline void VSM_init_funcHandlers(void) {
    broadcastHandler =      &VSM_broadcast_handler;
    messageHandler =        &VSM_message_handler;

    dataProcessHandler =    &VSM_data_process_handler;
    CANsendHandler =        &VSM_CANsendHandler;

    // Main States
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
    
    testingHandler =        &VSM_testingHandler;
}

bool VSM_init_periph(void) {
    VSM_init_funcHandlers();
    pinMode(LEFT_DOOR, INPUT);
    pinMode(RIGHT_DOOR, INPUT);
    left_door_state = digitalRead(LEFT_DOOR);
    right_door_state = digitalRead(RIGHT_DOOR);
    memset((void *) &temps, 0, sizeof(VSM_TEMPS));
    initADC();
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
inline void handleADC(void) {
    // ADC
    if (currIndex == 11) currIndex = 1;
    if  (!sampling) ADCstartSample(currIndex);
    if (READING_READY) {
        ADCread(&readings[currIndex-1]);
        currIndex++;
    }
}

void VSM_send_door_state(void) {
    setupBroadcast();
    sending->message_num = VSM_DOOR_STATE;
    sending->SIZE = 5;
    sending->byte0 = left_door_state;
    sending->byte1 = right_door_state;
    sending->byte2 = prev_lds;
    sending->byte3 = prev_rds;
    handleCANbco();
}

void VSM_temp1(void) {
    setupBroadcast();
    sending->message_num = VSM_TEMP1;
    sending->SIZE = 7;
    sending->byte0 = CABINH;
    sending->byte1 = CABINL;
    sending->byte2 = WCMH;
    sending->byte3 = WCML;
    sending->byte4 = F12VH;
    sending->byte5 = F12VL;
    handleCANbco();
}

void VSM_temp2(void) {
    setupBroadcast();
    sending->message_num = VSM_TEMP2;
    sending->SIZE = 7;
    sending->byte0 = B12VH;
    sending->byte1 = B12VL;
    sending->byte2 = F12VHH;
    sending->byte3 = F12VHL;
    sending->byte4 = B12VHH;
    sending->byte5 = B12VHL;
    handleCANbco();
}

/*
void VSM_Edata(void) {
    setupBroadcast();
    sending->message_num = VSM_EDATA;
    
    handleCANbco();
}
*/

void VSM_data_process_handler(void) {
    
    // Limit Switches
    prev_rds = right_door_state;
    prev_lds = left_door_state;
    left_door_state = digitalRead(LEFT_DOOR);
    right_door_state = digitalRead(RIGHT_DOOR);
    if (CHECK_DOORS) doorChanged = true;
    
    // ADC
    handleADC();
    
    if (timer45Event) {

        timer45Event = false;
    }
}

void VSM_CANsendHandler(void) {
    if (doorChanged) {
        VSM_send_door_state();
        doorChanged = false;
    }
    VSM_temp1();
    VSM_temp2();
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                    Module Specific State Behavior Handlers                 */
/******************************************************************************/
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

void VSM_testingHandler(void) {
    uint16_t reading = 0;
    uint8_t currIndex = 1;
    while (1) {
        if (currIndex == 11) currIndex = 1;
        if  (!sampling) ADCstartSample(currIndex);
        if (READING_READY) {
            ADCread(&reading);
            printf("(%2d) %4d\r", currIndex, reading);
            currIndex++;
        }
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
void VSM_printVariables(void) {
    int i;
    printf("Left Door: %s Right Door: %s\r\n", left_door_state ? "true" : "false", right_door_state ? "true" : "false");
    for (i = 0; i < 10; i++) printf("[%d]", readings[i]);
    printf("\r\n");
}
/******************************************************************************/
/******************************************************************************/

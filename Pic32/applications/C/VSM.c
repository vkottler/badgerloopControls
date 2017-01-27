#include "../include/VSM.h"

bool left_door_state, right_door_state;

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
    initADC(ourRole);
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
void VSM_data_process_handler(void) {
    left_door_state = digitalRead(LEFT_DOOR);
    right_door_state = digitalRead(RIGHT_DOOR);
    
    if (timer45Event) {

        timer45Event = false;
    }
}

void VSM_CANsendHandler(void) {
    
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
    while (1) {
        blinkBoardLights(2, 250);
        delay(500, MILLI);
        printf("loop\r\n");
    }
}
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                        Serial Debugging Utilities                          */
/******************************************************************************/
void VSM_printVariables(void) {
    printf("TODO\r\n");
}
/******************************************************************************/
/******************************************************************************/

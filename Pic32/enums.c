#include "enums.h"

const char *roleStr[] = {"NP", "WCM", "VNM", "BCM", "MCM", "VSM", "BMS"};

const char *airStr[] = {"INFL", "DEFL", "PURO"};

const char *stateStr[] = {
    "FAULT",			// SpaceX wants Fault   as value 0
    "DASH_CTL",			// SpaceX wants Idle    as value 1
    "RFL",              // SpaceX Wants Ready   as value 2
    "PUSH_PHASE",		// SpaceX wants Pushing as value 3
    "COAST",			// SpaceX wants Coast   as value 4
    "N_BRAKING", 		// SpaceX wants Braking as value 5
    "E_BRAKE",
    "FA_BRAKING",
    "RA_BRAKING",
    "WF_SAFE", "SAFE"  
};

const char *faultStr[] = {
    "HEALTHY", "GLOBAL_INITS",
    "LOCAL_INITS", "ILLEGAL_STATE",
    "ILLEGAL_ROLE", "UNINITIALIZED_HANDLER",
    "CAN_BUS", "CAN_IF", "CAN_OF", "UNIMPLEMENTED" 
};

const char *messageStr[] = {
    "INVALID",

    "HEARTBEAT", "PING_TO  ", "PING_BACK", "SOFTWARE_VER", "FAULT", "MODULE_STARTUP"  

    "VNM_POS", "VNM_VEL", "VNM_ACC", "VNM_ATT", "VNM_STRIPLOST",

    "VSM_TEMP1", "VSM_TEMP2", "VSM_EDATA",    

    "BCM_BRAKE_STATE", "BCM_BRAKE_SPEED1", "BCM_BRAKE_SPEED2",
 
    "MCM_KELLY_STATE", "MCM_CMDV", "MCM_HB_SPEED1", "MCM_HB_SPEED2",
            
    //OUT FROM DASH MESSAGE TYPES
    "ENTER_STATE",
    
    "DASH_VNM_POSINDEXRESET",

    "DASH_BCM_AIRACTUATE", "DASH_BCM_BRAKEACTUATE", "DASH_BCM_ABS_STATE",
    
    "DASH_MCM_SPINWHEELS",
    
    "DASH_VSM_CONTRACTOR"
};

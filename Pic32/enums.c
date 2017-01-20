#include "enums.h"

const char *roleStr[] = {
    "NP", "WCM", "VNM", "BCM", "MCM", "VSM", "BMS"
};

const char *stateStr[] = {
    "FAULT_STATE",		// SpaceX wants Fault   as value 0
    "DASH_CTL",			// SpaceX wants Idle    as value 1
    "READY_FOR_LAUNCH",		// SpaceX Wants Ready   as value 2
    "PUSH_PHASE",		// SpaceX wants Pushing as value 3
    "COAST",			// SpaceX wants Coast   as value 4
    "NORMAL_BRAKING", 		// SpaceX wants Braking as value 5
    "EMERGENCY_BRAKE",
    "FRONT_AXLE_BRAKING",
    "REAR_AXLE_BRAKING",
    "WAITING_FOR_SAFE", "SPINDOWN", "SAFE"  
};

const char *faultStr[] = {
    "HEALTHY", "GLOBAL_INITS",
    "LOCAL_INITS", "ILLEGAL_STATE",
    "ILLEGAL_ROLE", "UNINITIALIZED_HANDLER",
    "CAN_BUS", "CAN_INT" 
};

const char *messageStr[] = {
    "INVALID",
    "HEARTBEAT",
    "VNM_POS", "VNM_VEL", "VNM_ACC", "VNM_ATT",     
    "VSM_TEMP1", "VSM_TEMP2", "VSM_EDATA",    
    "BCM_BRAKE_STATE", "BCM_BRAKE_SPEED1", "BCM_BRAKE_SPEED2", 
    "MCM_KELLY_STATE", "MCM_CMDV", "MCM_HB_SPEED1", "MCM_HB_SPEED2",
    "VNM_STRIPLOST",
            
    //OUT FROM DASH MESSAGE TYPES
    "ENTER_STATE",
    
    "DASH_VNM_POSINDEXRESET",

    "DASH_BCM_AIRACTUATE", "DASH_BCM_BRAKEACTUATE", "DASH_BCM_ABS_STATE",
    
    "DASH_MCM_SPINWHEELS",
    
    "DASH_VSM_CONTRACTOR",    

    // NOT YET ADDED GLOBALLY (i.e. in spreadsheet)
    "PING_TO  ", "PING_BACK", "FAULT"   
};

#include "enums.h"

const char *roleStr[] = {
    "NP", "WCM", "VNM", "BCM", "MCM", "VSM", "BMS"
};

const char *stateStr[] = {
    "READY_FOR_LAUNCH", "DASH_CTL", "FAULT_STATE",
    "SAFE", "RUNNING", "EMERGENCY_BRAKE",
    "NORMAL_BRAKING", "FRONT_AXLE_BRAKING",
    "REAR_AXLE_BRAKING", "INFLATE",
    "WAITING_FOR_SAFE", "PUSH_PHASE",
    "COAST", "SPINDOWN"    
};

const char *faultStr[] = {
    "HEALTHY", 
    "GLOBAL_INITS_FAILED",
    "LOCAL_INIT_FAILED",
    "ILLEGAL_STATE",
    "ILLEGAL_ROLE",
    "UNINITIALIZED_HANDLER",
    "CAN_BUS_ERROR"    
};

const char *messageStr[] = {
    "INVALID",
    "WCM_HB", "VNM_HB", "BCM_HB", "MCM_HB", "VSM_HB",
    "VNM_POS", "VNM_VEL", "VNM_ACC", "VNM_ATT",     
    "VSM_TEMP1", "VSM_TEMP2", "VSM_EDATA",    
    "BCM_BRAKE_STATE", "BCM_BRAKE_SPEED1", "BCM_BRAKE_SPEED2", 
    "MCM_KELLY_STATE", "MCM_CMDV", "MCM_HB_SPEED1", "MCM_HB_SPEED2",
            
    // NOT YET ADDED GLOBALLY (i.e. in spreadsheet)
    "TEST_MSG", "PING", "FAULT"    
};

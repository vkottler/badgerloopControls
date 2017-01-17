#include "enums.h"

const char roleStr[] = {
    "NOT_PRESENT", "WCM", "VNM", "BCM", "MCM", "VSM", "BMS"
};

const char stateStr[] {
    "READY_FOR_LAUNCH", "DASH_CTL", "FAULT_STATE",
    "SAFE", "RUNNING", "EMERGENCY_BRAKE",
    "NORMAL_BRAKING", "FRONT_AXLE_BRAKING",
    "REAR_AXLE_BRAKING", "INFLATE",
    "WAITING_FOR_SAFE", "PUSH_PHASE",
    "COAST", "SPINDOWN"    
};

const char faultStr[] {
    "HEALTHY", 
    "GLOBAL_INITS_FAILED",
    "LOCAL_INIT_FAILED",
    "ILLEGAL_STATE",
    "ILLEGAL_ROLE",
    "UNINITIALIZED_HANDLER",
    "CAN_BUS_ERROR"    
};

const char messageStr[] {
    "INVALID",
    "WCM_HB", "VNM_HB", "VSM_HB", "BCM_HB", "MCM_HB", 
            
    // NOT YET ADDED GLOBALLY (i.e. in spreadsheet)
    "TEST_MSG", "PING", "FAULT"    
};

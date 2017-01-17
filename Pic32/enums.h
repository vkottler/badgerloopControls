#ifndef _ENUMS__H__
#define _ENUMS__H__
/******************************************************************************/
/* * * * * * *         GLOBALLY USED ENUMERATIONS           * * * * * * * * * */
/******************************************************************************/
typedef enum { NOT_PRESENT, WCM, VNM, BCM, MCM, VSM, BMS } ROLE;

typedef enum { 
    READY_FOR_LAUNCH, 
    DASH_CTL,
    FAULT_STATE,
    SAFE,
    RUNNING,
       
    // braking states
    EMERGENCY_BRAKE,
    NORMAL_BRAKING,
    FRONT_AXLE_BRAKING,
    REAR_AXLE_BRAKING,
    INFLATE,
    WAITING_FOR_SAFE,
            
    // wheel control states
    PUSH_PHASE,
    COAST,
    SPINDOWN
} STATE;

typedef enum { 
    HEALTHY, 
    GLOBAL_INITS_FAILED,
    LOCAL_INIT_FAILED,
    ILLEGAL_STATE,
    ILLEGAL_ROLE,
    UNINITIALIZED_HANDLER,
    CAN_BUS_ERROR
} FAULT_TYPE;

typedef enum {
    INVALID,
    WCM_HB, VNM_HB, VSM_HB, BCM_HB, MCM_HB, 
            
    // NOT YET ADDED GLOBALLY (i.e. in spreadsheet)
    TEST_MSG, PING, FAULT
} MESSAGE_TYPE;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/* * * * * * *             STRING REPRESENTATIONS           * * * * * * * * * */
/******************************************************************************/
extern const char *roleStr;
extern const char *stateStr;
extern const char *faultStr;
extern const char *messageStr;
/******************************************************************************/
/******************************************************************************/
#endif

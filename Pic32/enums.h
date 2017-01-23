#ifndef _ENUMS__H__
#define _ENUMS__H__

#include <string.h>
#include <stdint.h>
/******************************************************************************/
/* * * * * * *         GLOBALLY USED ENUMERATIONS           * * * * * * * * * */
/******************************************************************************/
typedef enum { NOT_PRESENT, WCM, VNM, BCM, MCM, VSM, BMS } ROLE;

#define NUM_ROLES       7

typedef enum { INFLATED, DEFLATED, PURGE_OPEN } AIR_SYSTEM_STATE;

typedef volatile enum { 
    FAULT_STATE,		// SpaceX wants Fault   as value 0
    DASH_CTL,			// SpaceX wants Idle    as value 1
    READY_FOR_LAUNCH,		// SpaceX Wants Ready   as value 2
    PUSH_PHASE,			// SpaceX wants Pushing as value 3
    COAST,			// SpaceX wants Coast   as value 4
    NORMAL_BRAKING, 		// SpaceX wants Braking as value 5
    EMERGENCY_BRAKE,
    FRONT_AXLE_BRAKING,
    REAR_AXLE_BRAKING,
    WAITING_FOR_SAFE,
    SAFE			      
} STATE;

#define NUM_STATES              11

typedef enum { 
    HEALTHY, 
            
    // Initialization / Software failure
    GLOBAL_INITS_FAILED, LOCAL_INIT_FAILED,
    ILLEGAL_STATE, ILLEGAL_ROLE, UNINITIALIZED_HANDLER,
            
    // CAN failure
    CAN_BUS_ERROR, CAN_IN_FULL_ERROR, CAN_OUT_FULL_ERROR,
    
    // Use to make sure we handle new faults if we add more
    UNIMPLEMENTED_FAULT
            
} FAULT_TYPE;

#define IS_CAN_FAULT            fault == CAN_BUS_ERROR || fault == CAN_IN_FULL_ERROR || fault == CAN_OUT_FULL_ERROR

#define NUM_FAULT_TYPES         9

typedef enum {
    INVALID,

    /**************************************************************************/
    /* * * * * * *             Out From Modules             * * * * * * * * * */
    /**************************************************************************/
    
    // Universal
    HEARTBEAT, PING_TO, PING_BACK, SOFTWARE_VER, FAULT, MODULE_STARTUP,
    
    // From VNM        
    VNM_POS, VNM_VEL, VNM_ACC, VNM_ATT, VNM_STRIPLOST,       
    
    // From VSM        
    VSM_TEMP1, VSM_TEMP2, VSM_EDATA,
            
    // From BCM
    BCM_BRAKE_STATE, BCM_BRAKE_SPEED1, BCM_BRAKE_SPEED2,
            
    // From MCM
    MCM_KELLY_STATE, MCM_CMDV, MCM_HB_SPEED1, MCM_HB_SPEED2,
    /**************************************************************************/
    /**************************************************************************/
  
  
    /**************************************************************************/
    /* * * * * * *                Out From WCM              * * * * * * * * * */
    /**************************************************************************/
    ENTER_STATE, 
            
    DASH_VNM_POSINDEXRESET,  
            
    DASH_BCM_AIRACTUATE, DASH_BCM_BRAKEACTUATE, DASH_BCM_ABS_STATE,
            
    DASH_MCM_SPINWHEELS,
            
    DASH_VSM_CONTRACTOR        
    /**************************************************************************/
    /**************************************************************************/

} MESSAGE_TYPE;

#define NUM_CAN_MESSAGES        29
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/* * * * * * *              Custom Structures               * * * * * * * * * */
/******************************************************************************/
typedef union {
    struct {
        
        // SID
        union {
            struct {
                unsigned SID:11;
                unsigned FILHIT:5;
                unsigned TS:16;
            };
            struct {
                unsigned :6; 
                unsigned from:3;
                unsigned priority:1;
                unsigned all:1;
                unsigned :21;
            };
            uint32_t SIDword;
        };
        
        // SIZE word
        unsigned SIZE:4;
        unsigned RBO:1;
        unsigned :3;
        unsigned RB1:1;
        unsigned RTR:1;
        unsigned EID:18;
        unsigned IDE:1;
        unsigned SRR:1;
        unsigned :2;
        
        // data word 1
        MESSAGE_TYPE message_num:8; 
        uint8_t byte0;
        uint8_t byte1; 
        uint8_t byte2;
        
        // data word 0
        uint8_t byte3; uint8_t byte4;
        uint8_t byte5; uint8_t byte6;
    };
    struct {
        unsigned :32; unsigned :32;
        uint32_t dataw0; uint32_t dataw1;
    };
    struct {
        unsigned :32; unsigned :32;
        unsigned :8;
        uint8_t bytes[7];
    };
    unsigned int raw[4];
} CAN_MESSAGE;

typedef struct {
    signed short ax;  
    signed short ay;
    signed short az;
    signed short gx;   
    signed short gy;
    signed short gz;
} COORD_VECTOR;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/* * * * * * *             STRING REPRESENTATIONS           * * * * * * * * * */
/******************************************************************************/
extern const char *roleStr[], *stateStr[], *faultStr[], *messageStr[], *stateStr[];
/******************************************************************************/
/******************************************************************************/
#endif

#ifndef _ENUMS__H__
#define _ENUMS__H__

#include <string.h>
#include <stdint.h>
/******************************************************************************/
/* * * * * * *         GLOBALLY USED ENUMERATIONS           * * * * * * * * * */
/******************************************************************************/
typedef enum { NOT_PRESENT, WCM, VNM, BCM, MCM, VSM, BMS } ROLE;

#define NUM_ROLES       7

// ! ! ! ! ! WARNING ! ! ! ! !
// If you add to this enum make sure to add an entry to the 
// array in enums.c, as well as increment the defined number
// for this type
//
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

#define NUM_STATES              14

// ! ! ! ! ! WARNING ! ! ! ! !
// If you add to this enum make sure to add an entry to the 
// array in enums.c, as well as increment the defined number
// for this type
//
typedef enum { 
    HEALTHY, 
    GLOBAL_INITS_FAILED,
    LOCAL_INIT_FAILED,
    ILLEGAL_STATE,
    ILLEGAL_ROLE,
    UNINITIALIZED_HANDLER,
    CAN_BUS_ERROR
} FAULT_TYPE;

#define NUM_FAULT_TYPES         7

// ! ! ! ! ! WARNING ! ! ! ! !
// If you add to this enum make sure to add an entry to the 
// array in enums.c, as well as increment the defined number
// for this type
//
typedef enum {
    INVALID,
    WCM_HB, VNM_HB, BCM_HB, MCM_HB, VSM_HB, // matches up with the value of ROLE
            
    // NOT YET ADDED GLOBALLY (i.e. in spreadsheet)
    TEST_MSG, PING, FAULT
} MESSAGE_TYPE;

#define NUM_CAN_MESSAGES        9
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
        uint8_t bytes[8];
    };
    unsigned int raw[4];
} CAN_MESSAGE;
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/* * * * * * *             STRING REPRESENTATIONS           * * * * * * * * * */
/******************************************************************************/
extern const char *roleStr[];
extern const char *stateStr[];
extern const char *faultStr[];
extern const char *messageStr[];
/******************************************************************************/
/******************************************************************************/
#endif

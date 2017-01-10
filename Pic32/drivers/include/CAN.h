#ifndef _CAN__H__
#define _CAN__H__

// Libraries
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/kmem.h>

#include "../../utils.h"
#include "../../globals.h"

// CAN settings
#define BAUD_250K           1
//#define BAUD_1M             1 // For interfacing with Kelly Controller
#define CAN_MAIN            1
#define CAN_ALT             2

// http://stackoverflow.com/questions/13923425/c-preprocessor-concatenation-with-variable
#define _CAN_SFR(reg, module)   C##module##reg    
#define CAN_SFR(reg, module)    _CAN_SFR(reg, module)

/*
 * Chosen FIFO usage:
 * 0: Receive, Mask: 0x400
 * 1: Receive, Mask: module SID bit
 * 2: Send, Specific Recipients
 * 3: Send, Broadcast (i.e. heartbeat)
 */

/*
 * Uses alternate set of pins (FCANIO = OFF)
 * CAN1: Pin 15 (AC1RX), Pin 14 (AC1TX)
 * CAN2: Pin 23 (AC2RX), Pin 22 (AC2TX)
 */

#define ID_FOR_KELLY        0x73
#define CAP_TIME            1

// Internal CAN Modes
#define LISTEN_ALL_MODE     7
#define CONFIG_MODE         4
#define LISTEN_ONLY_MODE    3
#define LOOPBACK_MODE       2
#define DISABLE_MODE        1
#define NORMAL_MODE         0

// SIDs from (http://bit.ly/2iQw5cs)
#define VNM_SID 0x001
#define VSM_SID 0x002
#define BCM_SID 0x004
#define MCM_SID 0x008
#define WCM_SID 0x010
#define BMS_SID 0x020
#define FLEX1   0x040
#define FLEX2   0x080
#define FLEX3   0x100
#define FLEX4   0x200
#define ALL     0x400

#define BUFFER_SIZE         4 // 4 * 4 bytes in a single message buffer
#define fifo_0_size         4 // *TEMPORARY* for initialization for motor test
#define fifo_1_size         4 // *TEMPORARY* for initialization for motor test

void CAN_init(ROLE role);
int CAN_check_error(void);
void CAN_send_message(uint32_t *message);
void CAN_receive_message(uint32_t *receive);
int CAN_message_available(void);

/* Create a CMSGSID data type. */
/*
typedef struct txcmsgsid {
    unsigned SID:11;
    unsigned :21;
} txcmsgsid;
*/

/* Create a CMSGEID data type. */
/*
typedef struct txcmsgeid {
    unsigned DLC:4;
    unsigned RB0:1;
    unsigned :3;
    unsigned RB1:1;
    unsigned RTR:1;
    unsigned EID:18;
    unsigned IDE:1;
    unsigned SRR:1;
    unsigned :2;
} txcmsgeid;
*/

/* Create a CMSGDATA0 data type. */
/*
typedef struct txcmsgdata0 {
    unsigned Byte0:8;
    unsigned Byte1:8;
    unsigned Byte2:8;
    unsigned Byte3:8;
} txcmsgdata0;
*/

/* Create a CMSGDATA1 data type. */
/*
typedef struct txcmsgdata1 {
    unsigned Byte4:8;
    unsigned Byte5:8;
    unsigned Byte6:8;
    unsigned Byte7:8;
} txcmsgdata1;
*/

/* This is the main data structure. */
/*
typedef union uCANTxMessageBuffer {
    struct {
        txcmsgsid CMSGSID;
        txcmsgeid CMSGEID;
        txcmsgdata0 CMSGDATA0;
        txcmsgdata0 CMSGDATA1;
    };
    uint32_t messageWord[4];
} uCANTxMessageBuffer;
*/

#endif

#ifndef _CAN__H__
#define _CAN__H__

// Libraries
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/kmem.h>
#include <string.h>

#include "../../utils.h"
#include "../../globals.h"

typedef union {
    struct {
        unsigned SIZE:5;
        unsigned SID:11;
        uint8_t message_num;
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
        uint8_t byte4;
        uint8_t byte5;
        uint8_t byte6;
        
    };
    struct {
        unsigned :16;
        uint8_t bytes[8];
    };
    struct {
        unsigned :16;
        uint32_t dataw0;
        uint32_t dataw1;
    };
    unsigned char raw_data[10];
} CAN_MESSAGE;

/*
 * Uses alternate set of pins (FCANIO = OFF)
 * CAN1: Pin 15 (AC1RX), Pin 14 (AC1TX)
 * CAN2: Pin 23 (AC2RX), Pin 22 (AC2TX)
 */

// CAN settings
#define BAUD_250K           1
//#define BAUD_1M             1 // For interfacing with Kelly Controller
#define DATA_ONLY           1
#define CAN_MAIN            1
#define CAN_ALT             2

// http://stackoverflow.com/questions/13923425/c-preprocessor-concatenation-with-variable
#define _CAN_SFR(reg, module)   C##module##reg    
#define CAN_SFR(reg, module)    _CAN_SFR(reg, module)

#if defined(DATA_ONLY)
#define BUFFER_SIZE         2   // 2 sets of 4 bytes in a single message buffer
#else
#define BUFFER_SIZE         4   // 4 sets of 4 bytes in a single message buffer
#endif
#define fifo_0_size         8
#define fifo_1_size         8
#define fifo_2_size         8
#define fifo_3_size         8
#define FIFO_SIZE ((fifo_0_size + fifo_1_size) * BUFFER_SIZE) + ((fifo_2_size + fifo_3_size) * 4)

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

void CAN_init(ROLE role);
int CAN_check_error(void);
void CAN_send(CAN_MESSAGE *message);
void CAN_broadcast(CAN_MESSAGE *message);
bool CAN_receive_broadcast(CAN_MESSAGE *message);
bool CAN_receive_specific(CAN_MESSAGE *message);

#endif

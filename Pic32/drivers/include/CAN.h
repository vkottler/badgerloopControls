#ifndef _CAN__H__
#define _CAN__H__

#ifdef SERIAL_DEBUG
#include <stdio.h>
#include "../../peripherals/include/ledShield.h"
#endif

/*
 * Uses alternate set of pins (FCANIO = OFF)
 * CAN1: Pin 15 (AC1RX), Pin 14 (AC1TX)
 * CAN2: Pin 23 (AC2RX), Pin 22 (AC2TX)
 */

// Libraries
#include <xc.h>
#include <sys/attribs.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/kmem.h>
#include <string.h>

#include "../../utils.h"
#include "../../globals.h"

/******************************************************************************/
/*                Structs, Enums and Compiler Directives                      */
/******************************************************************************/
typedef enum {
    INVALID,
    WCM_HB, VNM_HB, VSM_HB, BCM_HB, MCM_HB, 
    TEST_MSG, PING
            
} MESSAGE_TYPE;

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
                unsigned VNM_bit:1; 
                unsigned VSM_bit:1;
                unsigned BCM_bit:1; 
                unsigned MCM_bit:1;
                unsigned WCM_bit:1; 
                unsigned BMS_bit:1;
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
        MESSAGE_TYPE message_num:8; uint8_t byte0;
        uint8_t byte1; uint8_t byte2;
        
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

// http://stackoverflow.com/questions/13923425/c-preprocessor-concatenation-with-variable
#define _CAN_SFR(reg, module)   C##module##reg    
#define CAN_SFR(reg, module)    _CAN_SFR(reg, module)
/******************************************************************************/


/******************************************************************************/
/*                           Pre-build Settings                               */
/******************************************************************************/
#define BAUD_250K           1
//#define BAUD_1M             1 // For interfacing with Kelly Controller
#define CAP_TIME            1
#define CAN_MAIN            2
#define CAN_ALT             1

#if CAN_MAIN == 1
#define MAIN_CAN_VECTOR     _CAN_1_VECTOR
#define ALT_CAN_VECTOR      _CAN_2_VECTOR
#define MAIN_CAN_FLAG       IFS1bits.CAN1IF
#define ALT_CAN_FLAG        IFS1bits.CAN2IF
#else
#define MAIN_CAN_VECTOR     _CAN_2_VECTOR
#define ALT_CAN_VECTOR      _CAN_1_VECTOR
#define MAIN_CAN_FLAG       IFS1bits.CAN2IF
#define ALT_CAN_FLAG        IFS1bits.CAN1IF
#endif
/******************************************************************************/


/******************************************************************************/
/*                             FIFO Setup                                     */
/******************************************************************************/
#define fifo_0_size                 8
#define fifo_1_size                 8
#define fifo_2_size                 8
#define fifo_3_size                 8
#define FIFO_SIZE                   fifo_0_size + fifo_1_size + fifo_2_size + fifo_3_size
#define GLOBAL_RECEIVE_ENABLE       CAN_SFR(FIFOINT0bits, CAN_MAIN).RXNEMPTYIE
#define ADDRESSED_RECEIVE_ENABLE    CAN_SFR(FIFOINT1bits, CAN_MAIN).RXNEMPTYIE
#define GLOBAL_RECEIVE_FLAG         CAN_SFR(FIFOINT0bits, CAN_MAIN).RXNEMPTYIF
#define ADDRESSED_RECEIVE_FLAG      CAN_SFR(FIFOINT1bits, CAN_MAIN).RXNEMPTYIF
#define BROADCAST_REC_ADDR          PA_TO_KVA1(CAN_SFR(FIFOUA0, CAN_MAIN))
#define ADDRESSED_REC_ADDR          PA_TO_KVA1(CAN_SFR(FIFOUA1, CAN_MAIN))
#define ADDRESSED_SEND_ADDR         PA_TO_KVA1(CAN_SFR(FIFOUA2, CAN_MAIN))
#define BROADCAST_SEND_ADDR         PA_TO_KVA1(CAN_SFR(FIFOUA3, CAN_MAIN))
#define CAN_TIMER_FLAG              CAN_SFR(INTbits, CAN_MAIN).CTMRIF
#define CAN_TIMER_EN                CAN_SFR(INTbits, CAN_MAIN).CTMRIE
#define CAN_MAIN_VECTOR_BITS        CAN_SFR(VECbits, CAN_MAIN)
/******************************************************************************/


/******************************************************************************/
/*                     Internal CAN Mode Definitions                          */
/******************************************************************************/
#define LISTEN_ALL_MODE     7
#define CONFIG_MODE         4
#define LISTEN_ONLY_MODE    3
#define LOOPBACK_MODE       2
#define DISABLE_MODE        1
#define NORMAL_MODE         0
/******************************************************************************/


/******************************************************************************/
/*                      Module SIDs  (http://bit.ly/2iQw5cs)                  */
/******************************************************************************/
#define VNM_SID             0x001
#define VSM_SID             0x002
#define BCM_SID             0x004
#define MCM_SID             0x008
#define WCM_SID             0x010
#define BMS_SID             0x020
#define ALL                 0x400
#define ID_FOR_KELLY        0x73
/******************************************************************************/


/******************************************************************************/
/*                                 Functions                                  */
/******************************************************************************/
void CAN_init(ROLE role);
int CAN_check_error(void);
bool CAN_send(void);
bool CAN_broadcast(void);
bool CAN_receive_broadcast(CAN_MESSAGE *message);
bool CAN_receive_specific(CAN_MESSAGE *message);
bool CAN_message_is_heartbeat(CAN_MESSAGE *message);
/******************************************************************************/

#if (defined TESTING || defined PRODUCTION_TESTING) && defined SERIAL_DEBUG
void CAN_message_dump(CAN_MESSAGE *message, bool outgoing);
void CAN_print_errors(void);
#endif

#endif

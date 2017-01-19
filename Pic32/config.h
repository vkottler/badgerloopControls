#ifndef _CONFIG__H__
#define _CONFIG__H__

/******************************************************************************/
/* * * * * * *             Physical Configuration           * * * * * * * * * */
/******************************************************************************/
//#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used
#define PCB_PRESENT         1
//#define WCM_PRESENT         1

// Specify before building which board is doing what
// i.e. which board number has which PCB attached
#define BOARD1_ROLE         NOT_PRESENT
#define BOARD2_ROLE         MCM
#define BOARD3_ROLE         BCM
#define BOARD4_ROLE         NOT_PRESENT
#define BOARD5_ROLE         VSM
#define BOARD6_ROLE         VNM

#define SERIAL_DEBUG_BOARD  MCM
        
#ifndef WCM_PRESENT
#define HEARTBEAT_SENDER    BCM
#else
#define HEARTBEAT_SENDER    WCM
#endif

#define HEARTBEAT_DELAY     1000 // in ms
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                CAN Settings                                */
/******************************************************************************/
#define BAUD_250K           1
//#define BAUD_1M             1 // For interfacing with Kelly Controller
#define CAP_TIME            1
#define CAN_MAIN            1
#define CAN_ALT             2

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

#define fifo_0_size                 16
#define fifo_1_size                 16
#define fifo_2_size                 16
#define fifo_3_size                 16
#define FIFO_SIZE                   fifo_0_size + fifo_1_size + fifo_2_size + fifo_3_size
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
#endif

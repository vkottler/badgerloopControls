#ifndef _BUILD_CONFIG__H__
#define _BUILD_CONFIG__H__
/******************************************************************************/
/* * * * * * *          Software Build Definitions          * * * * * * * * * */
/******************************************************************************/
//#define SERIAL_DEBUG        1   // determines whether Serial will be usable
//#define TESTING             1   // this asserts that something in the TESTING section in main will be compiled
#define PRODUCTION          1   // use the production build (must still uncomment TESTING)

#ifdef PRODUCTION
//#define WCM_PRESENT         1
#define SERIAL_DEBUG_BOARD  VNM
#ifdef SERIAL_DEBUG_BOARD
#define CHECK_BOARD         ourRole == SERIAL_DEBUG_BOARD
#endif
#endif

#if (defined SERIAL_DEBUG || defined SERIAL_DEBUG_BOARD) && defined PRODUCTION
#define CAN_DUMP_OUT    1
#define CAN_DUMP_IN     1
#endif
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/* * * * * * *             Physical Configuration           * * * * * * * * * */
/******************************************************************************/
//#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used
#define PCB_PRESENT         1

// Specify before building which board is doing what
// i.e. which board number has which PCB attached
#if defined PRODUCTION
#define BOARD1_ROLE         NOT_PRESENT
#define BOARD2_ROLE         NOT_PRESENT
#define BOARD3_ROLE         NOT_PRESENT
#define BOARD4_ROLE         MCM
#define BOARD5_ROLE         NOT_PRESENT
#define BOARD6_ROLE         VNM

#ifndef WCM_PRESENT
#define HEARTBEAT_SENDER    MCM
#else
#define HEARTBEAT_SENDER    WCM
#endif

#ifdef HEARTBEAT_SENDER
#define HEARTBEAT_DELAY     1000 // in ms
#endif

#endif
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
#endif

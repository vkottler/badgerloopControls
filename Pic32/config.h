#ifndef _CONFIG__H__
#define _CONFIG__H__

/******************************************************************************/
/* * * * * * *             Physical Configuration           * * * * * * * * * */
/******************************************************************************/
//#define LED_SHIELD_PRESENT  1   // this asserts that the custom LED proto-shield will be present and can be used
#define PCB_PRESENT         1

#define BUILD_VERSION       v0.5.0

// Specify before building which board is doing what
// i.e. which board number has which PCB attached
#define BOARD1_ROLE         NOT_PRESENT
#define BOARD2_ROLE         NOT_PRESENT
#define BOARD3_ROLE         VNM
#define BOARD4_ROLE         NOT_PRESENT
#define BOARD5_ROLE         VSM
#define BOARD6_ROLE         MCM
#define BOARD7_ROLE         BCM
#define BOARD8_ROLE         NOT_PRESENT

#define DEBUG_DEFAULT       false
#define CAN_AUTOSEND        false

#define SEND_FAULT_TIMEOUT      1000    // ms
#define TICK_TIME               50

#define CAN_SEND_TICKS          20
#define CHECK_CAN_TICKS         ticks % CAN_SEND_TICKS == 0 && CAN_autosend

#define TMR45                   ((TMR5 << 16) | TMR4)
#define CHECK_SEND_CAN_TO       currTime - SEND_FAULT_TIMEOUT > lastSentTimeout && currTime > currTime - SEND_FAULT_TIMEOUT 

#define DEBUG_IC                1

//#define RUN_RDY             1
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
/*                                CAN Settings                                */
/******************************************************************************/
#define CAN_MAIN            1
#define CAN_ALT             2

#define BAUD_250K           1
//#define BAUD_1M             1 // For interfacing with Kelly Controller

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


/******************************************************************************/
/*                             Pre-Build Checks                               */
/******************************************************************************/
// hacky C preproc assert
// http://stackoverflow.com/questions/807244/c-compiler-asserts-how-to-implement
#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)
#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];


// This is an important check. If our MESSAGE_TYPE enum is greater than a byte
// it will not fit in the first byte of the CAN message. If the size of the 
// CAN message struct is greater than 16 bytes, Microchip's CAN peripheral will
// not interpret it properly or format incoming messages how we expect them to be.
CASSERT(sizeof(CAN_MESSAGE) == 16, enums_h);
CASSERT(sizeof(MESSAGE_TYPE) == 1, enums_h);

#define _TIMESTAMP(time, version, delim)  time#delim#version              
#define TIMESTAMP(version)         _TIMESTAMP(__TIMESTAMP__, version, _)
/******************************************************************************/
/******************************************************************************/     
#endif

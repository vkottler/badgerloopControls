#ifndef _CAN__H__
#define _CAN__H__

/*
 * Uses alternate set of pins (FCANIO = OFF)
 * CAN1: Pin 15 (AC1RX), Pin 14 (AC1TX)
 * CAN2: Pin 23 (AC2RX), Pin 22 (AC2TX)
 */

// Libraries
#include <xc.h>
#include <stdio.h>
#include <sys/attribs.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/kmem.h>
#include <string.h>

#include "../../globals.h"
#include "../../config.h"
#include "../../enums.h"


#include "../../peripherals/include/ledShield.h"

// http://stackoverflow.com/questions/13923425/c-preprocessor-concatenation-with-variable
#define _CAN_SFR(reg, module)   C##module##reg    
#define CAN_SFR(reg, module)    _CAN_SFR(reg, module)
#define BUS_OFF                 CAN_SFR(TRECbits, CAN_MAIN).TXBO


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


/******************************************************************************/
/*                                 Functions                                  */
/******************************************************************************/
void CAN_init(void);
int CAN_check_error(void);
void setupBroadcast(void);
void setupMessage(uint16_t SID);

inline void handleCANbco(void);
inline void handleCANmo(void);

bool CAN_send(void);
bool CAN_broadcast(void);

bool CAN_receive_broadcast(void);
bool CAN_receive_specific(void);

void CAN_send_fault(void);
void CAN_send_heartbeat(bool fake);

void CAN_message_dump(CAN_MESSAGE *message, bool outgoing);
void CAN_print_errors(void);
bool CAN_ping(uint16_t SID, bool initiator);
void CAN_message_dump(CAN_MESSAGE *message, bool outgoing);
void CAN_print_errors(void);
/******************************************************************************/
/******************************************************************************/
#endif

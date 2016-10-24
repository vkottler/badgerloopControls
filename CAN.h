#ifndef _CAN__H__
#define _CAN__H__

#include <xc.h>
#include <stdint.h>
#include <sys/kmem.h>
#include "ledShield.h"

/*
 * Uses alternate set of pins (FCANIO = OFF)
 * 
 * CAN1: Pin 15 (AC1RX), Pin 14 (AC1TX)
 * CAN2: Pin 23 (AC2RX), Pin 22 (AC2TX)
 */

#define ID_FOR_KELLY     0x73

#define LISTEN_ALL_MODE     7
#define CONFIG_MODE         4
#define LISTEN_ONLY_MODE    3
#define LOOPBACK_MODE       2
#define DISABLE_MODE        1
#define NORMAL_MODE         0

#define BUFFER_SIZE         4 // 4 * 4 bytes in a single message buffer
#define fifo_0_size         4 // *TEMPORARY* for initialization for motor test
#define fifo_1_size         4 // *TEMPORARY* for initialization for motor test

void CAN_init(void);
int CAN_check_error(void);
void CAN_send_message(uint32_t *message);
void CAN_receive_message(uint32_t *receive);

#endif

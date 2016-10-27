#ifndef _KELLYCONTROLLER__H__
#define _KELLYCONTROLLER__H__

#include <stdint.h>
#include "CAN.h"

#include <stdio.h>
#include <string.h>
#include "usbUART.h"

/*
 * Sending a command:
 * data[0] is the command
 * 
 * Receiving a response:
 * one or two data frames as response
 */

#define TORQUE_ID              0x6B // standard 11 bit IDs
#define SPEED_ID                100
#define INITIAL_RESPONSE_ID     0x73

// Constants
#define INFO_MODULE_NAME            64
#define INFO_SOFTWARE_VER           83
#define CAL_TPS_DEAD_ZONE_LOW       4
#define CAL_BRAKE_DEAD_ZONE_LOW     5
#define CAL_TPS_DEAD_ZONE_HIGH      38
#define CAL_BRAKE_DEAD_ZONE_HIGH    39
#define COM_READING                 0
#define CCP_INVALID_COMMAND         0xE3

typedef enum { 
    CCP_FLASH_READ1,
    CCP_FLASH_READ2,
    CCP_FLASH_READ3,
    CCP_FLASH_READ4,
    CCP_FLASH_READ5,
    CCP_FLASH_READ6,
    CCP_A2D_BATCH_READ1,
    CCP_A2D_BATCH_READ2,
    CCP_MONITOR1,
    CCP_MONITOR2,
    COM_SW_ACC,
    COM_SW_BRK,
    COM_SW_REV        
} COMMAND_NAME;

typedef struct {
    COMMAND_NAME name;
    uint8_t length;
    uint8_t *data;
    uint8_t resp_length;
} KELLY_CMD;

void Kelly_send(COMMAND_NAME cmd, int ID);
void Kelly_get_model(char *buffer, int ID);
void Kelly_get_software_ver(char *buffer, int ID);
void Kelly_get_throttle_low_high(char *buffer, int ID);
void Kelly_get_brake_low_high(char *buffer, int ID);
void Kelly_print_info(char *buffer, int ID);
void Kelly_print_batch1(char *buffer, int ID);
void Kelly_print_batch2(char *buffer, int ID);

#endif
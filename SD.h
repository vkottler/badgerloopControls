#ifndef _SD__H__
#define _SD__H__

#include "SPI.h"

/*
 * Uses SPI mode 0 (CPHA=0, CPOL=0)
 * Need a pull up resistor on DO
 *
 * The card is ready to receive a command frame when it drives DO high
 * R1, R2, R3, or R7 is sent back from the card
 * 
 * DI might be kept high during read (send 0xFF and get received data)
 * 
 * CS must be driven high to low prior to sending a command frame
 * and held low during the transaction
 * 
 * When it is initially powered on it is in native "SD" mode. 
 * Set DI and CS high and apply 74 or more clock pulses to SCLK
 * 
 * After sending CMD0, send a CMD8 with argument 0x000001AA
 *                        45       40 39      8 7   1
 * 48 bit commands: [0][1][commandNum][argument][CRC][1]
 * 
 */

#define GO_IDLE_STATE               0   // no arg                   response: R1
#define SEND_OP_COND                1   // no arg                   response: R1
#define SEND_CSD                    9   // no arg                   response: R1 w/ data
#define SEND_CID                    10  // no arg                   response: R1 w/ data
#define STOP_TRANSMISSION           12  // no arg                   response: R1b
#define SET_BLOCKLEN                16  // block length (uint32_t)  response: R1
#define READ_SINGLE_BLOCK           17  // address (uint32_t)       response: R1 w/ data
#define READ_MULTIPLE_BLOCK         18  // address (uint32_t)       response: R1 w/ data
#define WRITE_BLOCK                 24  // address (uint32_t)       response: R1
#define WRITE_MULTIPLE_BLOCK        25  // address (uint32_t)       response: R1
#define READ_OCR                    58  // no arg                   response: R3
/*
 * R1 response format: 0 X X X X X X X (0x00 generally means successful)
 * Bit 0: In Idle State
 * Bit 1: Erase Reset
 * Bit 2: Illegal Command
 * Bit 3: Command CRC Error
 * Bit 4: Erase Sequence Error
 * Bit 5: Address Error
 * Bit 6: Parameter Error
 * 
 * R1b: 
 * 
 * R3 response format [R1][ORC] where ORC is 32 bits
 */

#endif

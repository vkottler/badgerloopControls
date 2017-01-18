#ifndef _MCP4725__H__
#define _MCP4725__H__

#include "../../drivers/include/I2C.h"

#define MCP_ADDR         0x60
#define MCP4725_READ     0x01
#define MCP_WRITE_DAC    0x40
#define MCP_WRITE_EEPROM 0x60

#define MCP_PD_NORMAL    0b00000000 /* chip is enabled, and the output is active */
#define MCP_PD_1K        0b00000010 /* chip is shutdown, and the output is grounded with a 1k? resistor */
#define MCP_PD_100K      0b00000100 /* chip is shutdown, and the output is grounded with a 100k? resistor */
#define MCP_PD_500K      0b00000110 /* chip is shutdown, and the output is grounded with a 500k? resistor */

bool mcp_write_volt(float);
bool mcp_write_eeprom(float);

#endif

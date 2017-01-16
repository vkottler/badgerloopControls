#include "../include/MCP4725.h"

static uint8_t write_msg_data[3];

void mcp_write_val(uint16_t value) {
    value = (value > 4095) ? 4095 : value;
    
    write_msg_data[0] = MCP_WRITE_DAC | MCP_PD_NORMAL;
    uint16_t tmp = value >> 4;
    write_msg_data[1] = tmp;
    tmp = (value << 4) & 0x00FF;
    write_msg_data[2] = tmp;
    
    I2CwriteAndRead(MCP_ADDR, (buffer_t)write_msg_data, 3, NULL, 0);

    //i2c_master_xfer(I2C2, &write_msg, 1, 0);
    
}

void mcp_write_test2(void) {
    uint8_t dac_write[3];
    
    dac_write[0] = WRITEDAC;
    dac_write[1] = 0b11111111;
    dac_write[2] = 0b11110000;
    I2CwriteAndRead(MCP_ADDR, (buffer_t)dac_write, 3, NULL, 0);
}

void MCP4725_writeEeprom(int m_PowerMode, unsigned short value)
{
    //Create a temporary buffer
    char buff[5];
 
    //Block until the EEPROM is ready
    /* do {
        I2CwriteAndRead(MCP_ADDR, NULL, 0, buff, 3); 
    } while ((buff[0] & 0x80) == 0); */
 
    //Load the command, power mode, and 12-bit DAC value
    buff[0] = 0x60 | ((int)m_PowerMode << 1);
    buff[1] = value >> 4;
    buff[2] = value << 4;
 
    //Write the data
    I2CwriteAndRead(MCP_ADDR, buff, 3, NULL, 0); 
}

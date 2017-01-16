#include "../include/MCP4725.h"
uint8_t mcp[3];

// bool I2CwriteAndRead(unsigned int addr, const buffer_t write, unsigned int wlen, const buffer_t read, unsigned int rlen);

bool mcp_write_val(uint16_t value) {
    mcp[0] = MCP_WRITE_DAC;
    mcp[1] = value >> 4;
    mcp[2] = (value % 16) << 4;
    return !I2CwriteAndRead(MCP_ADDR, mcp, 3, NULL, 0); 
}
/**
 * 
 * @param voltage takes wanted voltage from the real interval [0, 4.9]
 * @return some convoluted 1 or 0 depending on success of I2C message
 */
bool mcp_write_volt(float voltage) {
    // convert volt to resolution. Math: 4096values/4.9V = 935.918 values/Volt
    float floatage = 1250.0f * (voltage);
    return mcp_write_val((int) floatage);
}

/*
void MCP4725_writeEeprom(int m_PowerMode, unsigned short value)
{
    //Create a temporary buffer
    char buff[5];
 
    //Block until the EEPROM is ready
    /* do {
        I2CwriteAndRead(MCP_ADDR, NULL, 0, buff, 3); 
    } while ((buff[0] & 0x80) == 0); 
 
    //Load the command, power mode, and 12-bit DAC value
    buff[0] = 0x60 | ((int)m_PowerMode << 1);
    buff[1] = value >> 4;
    buff[2] = value << 4;
 
    //Write the data
    I2CwriteAndRead(MCP_ADDR, buff, 3, NULL, 0); 
}
*/
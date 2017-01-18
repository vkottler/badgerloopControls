#include "../include/MCP4725.h"
uint8_t mcp[3];

// bool I2CwriteAndRead(unsigned int addr, const buffer_t write, unsigned int wlen, const buffer_t read, unsigned int rlen);

bool mcp_write_val(uint16_t value, bool writeEEPROM) {
    if (writeEEPROM) {
        mcp[0] = MCP_WRITE_EEPROM;
    } else {
        mcp[0] = MCP_WRITE_DAC;
    }
    mcp[1] = value >> 4;
    mcp[2] = (value % 16) << 4;
    return !I2CwriteAndRead(MCP_ADDR, mcp, 3, NULL, 0); 
}
/**
 * helper function converts value to voltage and sends it over.
 * @param voltage
 * @param writeEEPROM
 * @return 
 */
bool mcp_write_volt_helper(float voltage, bool writeEEPROM) {
    // convert volt to resolution. Math: 4096values/4.9V = 835.918 values/Volt
    uint32_t value = 836 * voltage;
    if (value > 0x0FFF) {
        return mcp_write_val(0x0FFF, writeEEPROM);
    }
    return mcp_write_val(voltage, writeEEPROM);
}

/**
 * writes voltage and doesn't write eeprom
 * @param voltage takes wanted voltage from the real interval [0, 4.9]
 * @return some convoluted 1 or 0 depending on success of I2C message
 */
bool mcp_write_volt(float voltage) {
    return mcp_write_volt_helper(voltage, false);
}

/**
 * writes voltage and eeprom (startup voltage)
 * @param voltage
 * @return some convoluted 1 or 0 depending on success of I2C message
 */
bool mcp_write_eeprom(float voltage) {
    return mcp_write_volt_helper(voltage, true);
}
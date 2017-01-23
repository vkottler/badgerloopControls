#include "../include/VL6180X.h"

int VL_singleShot(uint8_t addr) { return VL_setReg(addr, 0x018, 0x01); }
uint8_t VL_range(uint8_t addr) { return VL_read(addr, 0x062); }
void VL_clear_int(uint8_t addr) { VL_setReg(addr, 0x015, 0x07); }

uint8_t VL_sample(uint8_t addr) {
    uint8_t range;
    VL_singleShot(addr);
    VL_poll(addr); // horrible strategy
    range = VL_range(addr);
    VL_clear_int(addr);
    return range;
}

void VL_poll(uint8_t addr) {
    uint8_t range_status = 0;
    while (range_status != 0x04) range_status = (VL_read(addr, 0x04f)) & 0x07;
}

int VL_setReg(uint8_t addr, uint16_t reg, uint8_t val) {
    uint8_t values[3];
    values[0] = (reg & 0xff00) >> 8;
    values[1] = reg & 0x00ff;
    values[2] = val;
    return I2CwriteAndRead(addr, values, 3, NULL, 0, true); 
} 

uint8_t VL_read(uint8_t addr, uint16_t reg) {
    uint8_t values[2], retval = 0, errorCheck = 0;
    values[0] = (reg & 0xff00) >> 8;
    values[1] = reg & 0x00ff;
    errorCheck = I2CwriteAndRead(addr, values, 2, &retval, 1, true);
    if (errorCheck) return -1;
    return retval;
}

int VL_init(uint8_t addr) {
    uint8_t reset = VL_read(addr, 0x016);       // check if it has been initialized
    if (reset == -1) return -1;                 // I2C failed
    else if (reset) {
        // mandatory "suggested settings"
        VL_setReg(addr, 0x0207, 0x01);
        VL_setReg(addr, 0x0208, 0x01);
        VL_setReg(addr, 0x0096, 0x00);
        VL_setReg(addr, 0x0097, 0xfd);
        VL_setReg(addr, 0x00e3, 0x00);
        VL_setReg(addr, 0x00e4, 0x04);
        VL_setReg(addr, 0x00e5, 0x02);
        VL_setReg(addr, 0x00e6, 0x01);
        VL_setReg(addr, 0x00e7, 0x03);
        VL_setReg(addr, 0x00f5, 0x02);
        VL_setReg(addr, 0x00d9, 0x05);
        VL_setReg(addr, 0x00db, 0xce);
        VL_setReg(addr, 0x00dc, 0x03);
        VL_setReg(addr, 0x00dd, 0xf8);
        VL_setReg(addr, 0x009f, 0x00);
        VL_setReg(addr, 0x00a3, 0x3c);
        VL_setReg(addr, 0x00b7, 0x00);
        VL_setReg(addr, 0x00bb, 0x3c);
        VL_setReg(addr, 0x00b2, 0x09);
        VL_setReg(addr, 0x00ca, 0x09);
        VL_setReg(addr, 0x0198, 0x01);
        VL_setReg(addr, 0x01b0, 0x17);
        VL_setReg(addr, 0x01ad, 0x00);
        VL_setReg(addr, 0x00ff, 0x05);
        VL_setReg(addr, 0x0100, 0x05);
        VL_setReg(addr, 0x0199, 0x05);
        VL_setReg(addr, 0x01a6, 0x1b);
        VL_setReg(addr, 0x01ac, 0x3e);
        VL_setReg(addr, 0x01a7, 0x1f);
        VL_setReg(addr, 0x0030, 0x00);
        
        // recommended public register settings
        VL_setReg(addr, 0x0011, 0x10);              // Enables polling for ‘New Sample ready’
                                                    // when measurement completes
        VL_setReg(addr, 0x010a, 0x30);              // Set the averaging sample period
                                                    // (compromise between lower noise and
                                                    // increased execution time)
        VL_setReg(addr, 0x003f, 0x46);              // Sets the light and dark gain (upper
                                                    // nibble). Dark gain should not be
                                                    // changed.
        VL_setReg(addr, 0x0031, 0xFF);              // sets the # of range measurements after
                                                    // which auto calibration of system is
                                                    // performed
        VL_setReg(addr, 0x0040, 0x63);              // Set ALS integration time to 100ms
        VL_setReg(addr, 0x002e, 0x01);              // perform a single temperature calibration
                                                    // of the ranging sensor
        VL_setReg(addr, 0x001b, 0x09);              // Set default ranging inter-measurement
                                                    // period to 100ms
        VL_setReg(addr, 0x003e, 0x31);              // Set default ALS inter-measurement period
                                                    // to 500ms
        VL_setReg(addr, 0x0014, 0x24);              // Configures interrupt on ‘New Sample
                                                    // Ready threshold event’ 
      
        
        VL_setReg(addr, 0x016, 0x00);               // change fresh out of reset status
    }
    return 0;
}

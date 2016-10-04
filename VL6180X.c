#include "VL6180X.h"

char *errorMessage = "";
uint8_t errors[2];
uint8_t vlBuffer[255];

void VL_setReg(uint8_t address, uint16_t registerAddr, uint8_t value) { I2CwriteByteToRegister(address, registerAddr, value); }
void VL_setReg16(uint8_t address, uint16_t registerAddr, uint16_t value) { I2CwriteWordToRegister(address, registerAddr, value); }

uint8_t VL_getReg(uint8_t address, uint16_t registerAddr) { return I2CReadByteFromRegister(address, registerAddr); }

void initVL(uint8_t address) {
    VL_setReg(address, 0x0207, 0x01);
    VL_setReg(address, 0x0208, 0x01);
    VL_setReg(address, 0x0096, 0x00);
    VL_setReg(address, 0x0097, 0xfd);
    VL_setReg(address, 0x00e3, 0x00);
    VL_setReg(address, 0x00e4, 0x04);
    VL_setReg(address, 0x00e5, 0x02);
    VL_setReg(address, 0x00e6, 0x01);
    VL_setReg(address, 0x00e7, 0x03);
    VL_setReg(address, 0x00f5, 0x02);
    VL_setReg(address, 0x00d9, 0x05);
    VL_setReg(address, 0x00db, 0xce);
    VL_setReg(address, 0x00dc, 0x03);
    VL_setReg(address, 0x00dd, 0xf8);
    VL_setReg(address, 0x009f, 0x00);
    VL_setReg(address, 0x00a3, 0x3c);
    VL_setReg(address, 0x00b7, 0x00);
    VL_setReg(address, 0x00bb, 0x3c);
    VL_setReg(address, 0x00b2, 0x09);
    VL_setReg(address, 0x00ca, 0x09);  
    VL_setReg(address, 0x0198, 0x01);
    VL_setReg(address, 0x01b0, 0x17);
    VL_setReg(address, 0x01ad, 0x00);
    VL_setReg(address, 0x00ff, 0x05);
    VL_setReg(address, 0x0100, 0x05);
    VL_setReg(address, 0x0199, 0x05);
    VL_setReg(address, 0x01a6, 0x1b);
    VL_setReg(address, 0x01ac, 0x3e);
    VL_setReg(address, 0x01a7, 0x1f);
    VL_setReg(address, 0x0030, 0x00);
    VL_defautSettings(address);
    if (I2CcheckError()) { 
        sprintf(errorMessage, "VL %d did not initialize: ", address);
        println(errorMessage);
        I2CprintError();
    }
    //VL_checkStatus();
}

void VL_defautSettings(uint8_t address) {
  //Recommended settings from datasheet

  //Enable Interrupts on Conversion Complete (any source)
  VL_setReg(address, VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, (4 << 3)|(4) );          // Set GPIO1 high when sample complete
  VL_setReg(address, VL6180X_SYSTEM_MODE_GPIO1, 0x10);                              // Set GPIO1 high when sample complete
  VL_setReg(address, VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD, 0x30);                //Set Avg sample period
  VL_setReg(address, VL6180X_SYSALS_ANALOGUE_GAIN, 0x46);                           // Set the ALS gain
  VL_setReg(address, VL6180X_SYSRANGE_VHV_REPEAT_RATE, 0xFF);                       // Set auto calibration period (Max = 255)/(OFF = 0)
  VL_setReg(address, VL6180X_SYSALS_INTEGRATION_PERIOD, 0x63);                      // Set ALS integration time to 100ms
  VL_setReg(address, VL6180X_SYSRANGE_VHV_RECALIBRATE, 0x01);                       // perform a single temperature calibration
  
  //Optional settings from datasheet
  VL_setReg(address, VL6180X_SYSRANGE_INTERMEASUREMENT_PERIOD, 0x09);               // Set default ranging inter-measurement period to 100ms
  VL_setReg(address, VL6180X_SYSALS_INTERMEASUREMENT_PERIOD, 0x0A);                 // Set default ALS inter-measurement period to 100ms
  VL_setReg(address, VL6180X_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x24);                   // Configures interrupt on ?New Sample Ready threshold event? 
  
  //Additional settings defaults from community
  VL_setReg(address, VL6180X_SYSRANGE_MAX_CONVERGENCE_TIME, 0x32);
  VL_setReg(address, VL6180X_SYSRANGE_RANGE_CHECK_ENABLES, 0x10 | 0x01);
  VL_setReg16(address, VL6180X_SYSRANGE_EARLY_CONVERGENCE_ESTIMATE, 0x7B );
  VL_setReg16(address, VL6180X_SYSALS_INTEGRATION_PERIOD, 0x64);

  VL_setReg(address, VL6180X_READOUT_AVERAGING_SAMPLE_PERIOD,0x30);
  VL_setReg(address, VL6180X_SYSALS_ANALOGUE_GAIN,0x40);
  VL_setReg(address, VL6180X_FIRMWARE_RESULT_SCALER,0x01);
  
  //VL_setReg(address, VL6180X_SYSTEM_FRESH_OUT_OF_RESET, 0);
  
  //VL_setReg16(address, VL6180X_SYSRANGE_THRESH_HIGH,0xFF);                          // 20 cm
  //VL_setReg16(address, VL6180X_SYSRANGE_THRESH_LOW,0x00);                           // 5 cm
  
  //VL_setReg16(address, 0x025, 0xFF); //SYSRANGE__RANGE_IGNORE_VALID_HEIGHT
  //VL_setReg16(address, 0x02C, 0xFF); //SYSRANGE__MAX_AMBIENT_LEVEL_MULT - 0x02C
  //I2C16bitWriteByte(self->address, 0x02D, 0x00); //SYSRANGE__RANGE_CHECK_ENABLES - 0x02D
  //I2C16bitWriteByte(self->address, 0x021, 0x00); //SYSRANGE__CROSSTALK_VALID_HEIGHT - 0x21
  //VL_setReg16(address, 0x024, 0x04); //SYSRANGE__PART_TO_PART_RANGE_OFFSET - 0x24
  
  
}

uint8_t VL_getDistance(uint8_t address) {
    uint8_t retval = 0x00;
    VL_setReg(address, VL6180X_SYSRANGE_START, 0x01);
    delay(7);                                                   // with this implementation this is as short as you can wait and have it still work!
    retval = VL_getReg(address, VL6180X_RESULT_RANGE_VAL);
    //VL_setReg(0x15, 0x07); // need to clear interrupt status bit for range only
    return retval;
}

void updateErrors(uint8_t address) { I2CReadConsecutiveRegisters(address, 0x4D, 2, errors); }

void VL_checkStatus(uint8_t address) {
    updateErrors(address);
    switch ((errors[0] & 0xf0) >> 4) {
        case 0:
            errorMessage = "No error.";
            break;
        case 1:
            errorMessage = "VCSEL Continuity Test";
            break;
        case 2:
            errorMessage = "VCEL Watchdog Test";
            break;
        case 3:
            errorMessage = "VCEL Watchdog";
            break;
        case 4:
            errorMessage = "PLL1 Lock";
            break;
        case 5:
            errorMessage = "PLL2 Lock";
            break;
        case 6:
            errorMessage = "Early Convergence Estimate";
            break;
        case 7:
            errorMessage = "Max Convergence";
            break;
        case 8:
            errorMessage = "No Target Ignore";
            break;
        case 11:
            errorMessage = "Max Signal To Noise Ratio";
            break;
        case 12:
            errorMessage = "Raw Ranging Algo Underflow";
            break;
        case 13:
            errorMessage = "Raw Ranging Algo Overflow";
            break;
        case 14:
            errorMessage = "Ranging Algo Underflow";
            break;
        case 15:
            errorMessage = "Ranging Algo Overflow";
            break;
    }
    print("Range: ");
    println(errorMessage);
    switch (errors[1]) {
        case 0:
            errorMessage = "No error.";
            break;
        case 1:
            errorMessage = "Overflow error";
            break;
        case 2:
            errorMessage = "Underflow error";
            break;
    }
    print("ALS: ");
    println(errorMessage);
}
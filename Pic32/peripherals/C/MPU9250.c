#include "../include/MPU9250.h"

bool I2CWriteBits(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t bitStart, uint8_t length, uint8_t data) {
    uint8_t b;
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    uint8_t b_array[2];
    data <<= (bitStart - length + 1);
    data &= mask;
    b &= ~(mask);
    b |= data;
    b_array[0] = deviceRegister;
    b_array[1] = b;
    return I2CwriteAndRead(deviceAddress, b_array, 2, NULL, 0);
}

bool I2CWriteBit(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    uint8_t b_array[2];
    b_array[0] = deviceRegister;
    b_array[1] = b;
    return I2CwriteAndRead(deviceAddress, b_array, 2, NULL, 0);
}

bool MPUinitialize(void) {
    bool status;
    status |= I2CWriteBit(MPU_ADDRESS, MPU9250_PWR_MGMT_1, MPU9250_PWR1_SLEEP_BIT, 0);                                                               // Disable Sleep
    status |= I2CWriteBits(MPU_ADDRESS, MPU9250_PWR_MGMT_1, MPU9250_PWR1_CLKSEL_BIT, MPU9250_PWR1_CLKSEL_LENGTH, MPU9250_CLOCK_PLL_XGYRO);           // Set clock source
    status |= I2CWriteBits(MPU_ADDRESS, MPU9250_RA_GYRO_CONFIG, MPU9250_GCONFIG_FS_SEL_BIT, MPU9250_GCONFIG_FS_SEL_LENGTH, MPU9250_GYRO_FS_500);     // Set full scale gyro range
    status |= I2CWriteBits(MPU_ADDRESS, MPU9250_RA_ACCEL_CONFIG, MPU9250_ACONFIG_AFS_SEL_BIT, MPU9250_ACONFIG_AFS_SEL_LENGTH, MPU9250_ACCEL_FS_2);   // Set full scale accel range
    return !status;
}

bool MPUread(COORD_VECTOR *vec) {
    uint8_t readAccel[14];
    uint8_t readAccel_cmd = MPU9250_ACCEL_XOUT_H;
    if (I2CwriteAndRead(MPU_ADDRESS, &readAccel_cmd, 1, readAccel, 14)) return false;
    vec->ax = (((signed short) readAccel[0]) << 8) | readAccel[1];   // ax
    vec->ay = (((signed short) readAccel[2]) << 8) | readAccel[3];   // ay
    vec->az = (((signed short) readAccel[4]) << 8) | readAccel[5];   // az
    vec->gx = (((signed short) readAccel[8]) << 8) | readAccel[9];   // gx
    vec->gy = (((signed short) readAccel[10]) << 8) | readAccel[11]; // gy
    vec->gz = (((signed short) readAccel[12]) << 8) | readAccel[13]; // gz
    return true;    
}

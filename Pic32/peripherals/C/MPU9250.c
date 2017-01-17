#include "../include/MPU9250.h"
#include "../../globals.h"

#define MPU9250_PWR1_DEVICE_RESET_BIT   7
#define MPU9250_PWR1_SLEEP_BIT          6
#define MPU9250_PWR1_CYCLE_BIT          5
#define MPU9250_PWR1_TEMP_DIS_BIT       3
#define MPU9250_PWR1_CLKSEL_BIT         2
#define MPU9250_PWR1_CLKSEL_LENGTH      3

#define MPU9250_CLOCK_PLL_XGYRO         0x01

#define MPU9250_GYRO_FS_250         0x00
#define MPU9250_GYRO_FS_500         0x01
#define MPU9250_GYRO_FS_1000        0x02
#define MPU9250_GYRO_FS_2000        0x03

#define MPU9250_ACCEL_FS_2          0x00
#define MPU9250_ACCEL_FS_4          0x01
#define MPU9250_ACCEL_FS_8          0x02
#define MPU9250_ACCEL_FS_16         0x03

#define MPU9250_RA_GYRO_CONFIG      0x1B

#define MPU9250_GCONFIG_FS_SEL_BIT      4
#define MPU9250_GCONFIG_FS_SEL_LENGTH   2

#define MPU9250_RA_ACCEL_CONFIG     0x1C

#define MPU9250_ACONFIG_XA_ST_BIT           7
#define MPU9250_ACONFIG_YA_ST_BIT           6
#define MPU9250_ACONFIG_ZA_ST_BIT           5
#define MPU9250_ACONFIG_AFS_SEL_BIT         4
#define MPU9250_ACONFIG_AFS_SEL_LENGTH      2
#define MPU9250_ACONFIG_ACCEL_HPF_BIT       2
#define MPU9250_ACONFIG_ACCEL_HPF_LENGTH    3

bool I2CWriteBits(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t bitStart, uint8_t length, uint8_t data) {
    uint8_t b;
    
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1);
    data &= mask;
    b &= ~(mask);
    b |= data;
    
    uint8_t b_array[2];
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

void MPUinitialize(void) {
    int status;
    // Disable Sleep
    status = I2CWriteBit(MPU_ADDRESS, MPU9250_PWR_MGMT_1, MPU9250_PWR1_SLEEP_BIT, 0);
#if defined SERIAL_DEBUG
    printf("MPUinitialize - All status values should = 0\n");
    printf("MPU9250 disable sleep status: %d\n", status);
#endif
    // Set clock source
    status = I2CWriteBits(MPU_ADDRESS, MPU9250_PWR_MGMT_1, MPU9250_PWR1_CLKSEL_BIT, MPU9250_PWR1_CLKSEL_LENGTH, MPU9250_CLOCK_PLL_XGYRO);
#if defined SERIAL_DEBUG
    printf("MPU9250 set clock source status: %d\n", status);
#endif
    // Set full scale gyro range
    status = I2CWriteBits(MPU_ADDRESS, MPU9250_RA_GYRO_CONFIG, MPU9250_GCONFIG_FS_SEL_BIT, MPU9250_GCONFIG_FS_SEL_LENGTH, MPU9250_GYRO_FS_500);
#if defined SERIAL_DEBUG
    printf("MPU9250 set full scale gyro range status: %d\n", status);
#endif
    // Set full scale accel range
    status = I2CWriteBits(MPU_ADDRESS, MPU9250_RA_ACCEL_CONFIG, MPU9250_ACONFIG_AFS_SEL_BIT, MPU9250_ACONFIG_AFS_SEL_LENGTH, MPU9250_ACCEL_FS_2);
#if defined SERIAL_DEBUG
    printf("MPU9250 set full scale accel range status: %d\n", status);
#endif
}

void MPU9250_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    uint8_t readAccel_cmd[1];
    readAccel_cmd[0] = MPU9250_ACCEL_XOUT_H;
    
    uint8_t readAccel[14];
    
    int status = I2CwriteAndRead(MPU_ADDRESS, readAccel_cmd, 1, readAccel, 14);
    printf("MPU9250_getMotion6 Status: %d\n", status);
    
    *ax = (((int16_t)readAccel[0]) << 8) | readAccel[1]; // ax
    *ay = (((int16_t)readAccel[2]) << 8) | readAccel[3]; // ay
    *az = (((int16_t)readAccel[4]) << 8) | readAccel[5]; // az
    *gx = (((int16_t)readAccel[8]) << 8) | readAccel[9]; // gx
    *gy = (((int16_t)readAccel[10]) << 8) | readAccel[11]; // gy
    *gz = (((int16_t)readAccel[12]) << 8) | readAccel[13]; // gz 
    
}
void MPUread(void) {
    
}
void MPUgetPos(COORD_VECTOR* pos) {
    
}
void MPUgetVel(COORD_VECTOR* vel) {
    
}
void MPUgetAcc(COORD_VECTOR* acc) { 
    
}

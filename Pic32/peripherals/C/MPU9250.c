#include "../include/MPU9250.h"

// cm/s
// cm/s2

float accelBias[3], gyroBias[3];

unsigned int fifoCount = 0;

uint8_t addr = 0;
volatile uint8_t fifoCount[2];
uint16_t totalCount;
uint8_t readIndex = 0;

MPU_STATE mpuState = IDLE;

bool MPU_ready = false;

volatile uint8_t mpuBytes[14];

inline void startFIFOread(void) {
    addr = FIFO_COUNTH;
    I2CwriteAndRead(MPU_ADDRESS, &addr, 1, fifoCount, 2, false);
}

void MPU_step(void) {
    switch (mpuState) {
        case IDLE:
            MPU_startSampling();
            mpuState = WAIT;
            break;
        case WAIT:
            break;
            if (transactionReady && I2Csuccessful) {
                startFIFOread();
                mpuState = GET_FIFO_COUNT;
            }
            break;
        case GET_FIFO_COUNT:
            if (transactionReady && I2Csuccessful) {
                totalCount = (uint16_t) fifoCount[0] << 8 | (fifoCount[1]);
                mpuState = GET_VALUES;
            }
        case GET_VALUES:
            if (transactionReady && I2Csuccessful)
            break;
    }
}

void printOffsets(void) {
    printf("ax: %f ay: %f az: %f gx: %f gy: %f gz: %f\r\n", accelBias[0], accelBias[1], accelBias[2], gyroBias[0], gyroBias[1], gyroBias[2]);
}

inline void MPU_startSampling(void) {
    if (MPUwriteReg(FIFO_EN, 0x78, false)) fault = I2C_FAULT;
}

bool MPU_stopSampling(void) {
    if (MPUwriteReg(FIFO_EN, 0x00, false)) fault = I2C_FAULT;
}

bool MPUwriteReg(uint8_t reg, uint8_t value, bool block) {
    uint8_t vals[2];
    vals[0] = reg;
    vals[1] = value;
    return I2CwriteAndRead(MPU_ADDRESS, vals, 2, NULL, 0, block);
}

// Function which accumulates gyro and accelerometer data after device
// initialization. It calculates the average of the at-rest readings and then
// loads the resulting offsets into accelerometer and gyro bias registers.
bool calibrateMPU9250(float * gyroBias, float * accelBias) {
    uint8_t addr;
  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384; // = 16384 LSB/g 
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};
  int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};

  // reset device
  // Write a one to bit 7 reset bit; toggle reset device
  if (MPUwriteReg(PWR_MGMT_1, 0x80)) return false;
  delay(100, MILLI);

  // get stable time source; Auto select clock source to be PLL gyroscope
  // reference if ready else use the internal oscillator, bits 2:0 = 001
  MPUwriteReg(PWR_MGMT_1, 0x01);
  MPUwriteReg(PWR_MGMT_2, 0x00);
  delay(200, MILLI);

  // Configure device for bias calculation
  // Disable all interrupts
  MPUwriteReg(INT_ENABLE, 0x00);
  // Disable FIFO
  MPUwriteReg(FIFO_EN, 0x00);
  // Turn on internal clock source
  MPUwriteReg(PWR_MGMT_1, 0x00);
  // Disable I2C master
  MPUwriteReg(I2C_MST_CTRL, 0x00);
  // Disable FIFO and I2C master modes
  MPUwriteReg(USER_CTRL, 0x00);
  // Reset FIFO and DMP
  MPUwriteReg(USER_CTRL, 0x0C);
  delay(15, MILLI);

  // Configure MPU6050 gyro and accelerometer for bias calculation
  // Set low-pass filter to 188 Hz
  MPUwriteReg(CONFIG, 0x01);
  // Set sample rate to 1 kHz
  MPUwriteReg(SMPLRT_DIV, 0x00);
  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  MPUwriteReg(GYRO_CONFIG, 0x00);
  // Set accelerometer full-scale to 2 g, maximum sensitivity
  MPUwriteReg(ACCEL_CONFIG, 0x00);

  // Configure FIFO to capture accelerometer and gyro data for bias calculation
  MPUwriteReg(USER_CTRL, 0x40);  // Enable FIFO
  // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in
  // MPU-9150)
  MPUwriteReg(FIFO_EN, 0x78);
  delay(40, MILLI);  // accumulate 40 samples in 40 milliseconds = 480 bytes

  // At end of sample accumulation, turn off FIFO sensor read
  // Disable gyro and accelerometer sensors for FIFO
  MPUwriteReg(FIFO_EN, 0x00);
  // Read FIFO sample count
  addr = FIFO_COUNTH;
  I2CwriteAndRead(MPU_ADDRESS, &addr, 1, data, 2, true);
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  // How many sets of full gyro and accelerometer data for averaging
  packet_count = fifo_count/12;

  for (ii = 0; ii < packet_count; ii++) {
    
    // Read data for averaging
    addr = FIFO_R_W;
    I2CwriteAndRead(MPU_ADDRESS, &addr, 1, data, 12, true);
    // Form signed 16-bit integer for each sample in FIFO
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  );
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  );
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  );
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  );
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  );
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]);

    // Sum individual signed 16-bit biases to get accumulated signed 32-bit
    // biases.
    accel_bias[0] += (int32_t) accel_temp[0];
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
  }
  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  accel_bias[0] /= (int32_t) packet_count;
  accel_bias[1] /= (int32_t) packet_count;
  accel_bias[2] /= (int32_t) packet_count;
  gyro_bias[0]  /= (int32_t) packet_count;
  gyro_bias[1]  /= (int32_t) packet_count;
  gyro_bias[2]  /= (int32_t) packet_count;

  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  if (accel_bias[2] > 0L) accel_bias[2] -= (int32_t) accelsensitivity;
  else accel_bias[2] += (int32_t) accelsensitivity;

  // Construct the gyro biases for push to the hardware gyro bias registers,
  // which are reset to zero upon device startup.
  // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input
  // format.
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF;
  // Biases are additive, so change sign on calculated average gyro biases
  data[1] = (-gyro_bias[0]/4)       & 0xFF;
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

  // Push gyro biases to hardware registers
  MPUwriteReg(XG_OFFSET_H, data[0]);
  MPUwriteReg(XG_OFFSET_L, data[1]);
  MPUwriteReg(YG_OFFSET_H, data[2]);
  MPUwriteReg(YG_OFFSET_L, data[3]);
  MPUwriteReg(ZG_OFFSET_H, data[4]);
  MPUwriteReg(ZG_OFFSET_L, data[5]);

  // Output scaled gyro biases for display in the main program
  gyroBias[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
  gyroBias[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  gyroBias[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

  // Construct the accelerometer biases for push to the hardware accelerometer
  // bias registers. These registers contain factory trim values which must be
  // added to the calculated accelerometer biases; on boot up these registers
  // will hold non-zero values. In addition, bit 0 of the lower byte must be
  // preserved since it is used for temperature compensation calculations.
  // Accelerometer bias registers expect bias input as 2048 LSB per g, so that
  // the accelerometer biases calculated above must be divided by 8.

  // A place to hold the factory accelerometer trim biases
  int32_t accel_bias_reg[3] = {0, 0, 0};
  // Read factory accelerometer trim values
  addr = XA_OFFSET_H;
  I2CwriteAndRead(MPU_ADDRESS, &addr, 1, data, 2, true);
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  addr = YA_OFFSET_H;
  I2CwriteAndRead(MPU_ADDRESS, &addr, 1, data, 2, true);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  addr = ZA_OFFSET_H;
  I2CwriteAndRead(MPU_ADDRESS, &addr, 1, data, 2, true);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

  // Define mask for temperature compensation bit 0 of lower byte of
  // accelerometer bias registers
  uint32_t mask = 1uL;
  // Define array to hold mask bit for each accelerometer bias axis
  uint8_t mask_bit[3] = {0, 0, 0};

  for (ii = 0; ii < 3; ii++)
  {
    // If temperature compensation bit is set, record that fact in mask_bit
    if ((accel_bias_reg[ii] & mask))
    {
      mask_bit[ii] = 0x01;
    }
  }

  // Construct total accelerometer bias, including calculated average
  // accelerometer bias from above
  // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g
  // (16 g full scale)
  accel_bias_reg[0] -= (accel_bias[0]/8);
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  // preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[1] = data[1] | mask_bit[0];
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[3] = data[3] | mask_bit[1];
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[5] = data[5] | mask_bit[2];

  // Apparently this is not working for the acceleration biases in the MPU-9250
  // Are we handling the temperature correction bit properly?
  // Push accelerometer biases to hardware registers
  MPUwriteReg(XA_OFFSET_H, data[0]);
  MPUwriteReg(XA_OFFSET_L, data[1]);
  MPUwriteReg(YA_OFFSET_H, data[2]);
  MPUwriteReg(YA_OFFSET_L, data[3]);
  MPUwriteReg(ZA_OFFSET_H, data[4]);
  MPUwriteReg(ZA_OFFSET_L, data[5]);

  // Output scaled accelerometer biases for display in the main program
  accelBias[0] = (float)accel_bias[0]/(float)accelsensitivity;
  accelBias[1] = (float)accel_bias[1]/(float)accelsensitivity;
  accelBias[2] = (float)accel_bias[2]/(float)accelsensitivity;
  
  return true;
}

bool MPUinitialize(void) {
    return calibrateMPU9250(gyroBias, accelBias);
}

bool MPUread(void) {
    uint8_t readAccel_cmd = ACCEL_XOUT_H;
    if (I2CwriteAndRead(MPU_ADDRESS, &readAccel_cmd, 1, mpuBytes, 14, false)) return false;
    return true;     
}

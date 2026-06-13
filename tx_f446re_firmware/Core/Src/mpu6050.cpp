/*
 * mpu6050.cpp
 *
 *  Created on: Jun 7, 2026
 *      Author: yufur
 */




#include "mpu6050.h"
#include "i2c_driver.h"

#define MPU6050_ADDR    0x68
#define WHO_AM_I_REG    0x75
#define PWR_MGMT_1_REG  0x6B

void MPU6050::init() {

    I2C::writeByte(MPU6050_ADDR, PWR_MGMT_1_REG, 0x00);
}


void MPU6050::readAll(MPU6050_Data* dataStruct) {
	uint8_t rawData[14] = {0};

    I2C::readBytes(MPU6050_ADDR, ACCEL_XOUT_H_REG, 14, rawData);



    dataStruct->Accel_X = (int16_t)(rawData[0] << 8 | rawData[1]);
    dataStruct->Accel_Y = (int16_t)(rawData[2] << 8 | rawData[3]);
    dataStruct->Accel_Z = (int16_t)(rawData[4] << 8 | rawData[5]);

    dataStruct->Temp    = (int16_t)(rawData[6] << 8 | rawData[7]);

    dataStruct->Gyro_X  = (int16_t)(rawData[8] << 8 | rawData[9]);
    dataStruct->Gyro_Y  = (int16_t)(rawData[10] << 8 | rawData[11]);
    dataStruct->Gyro_Z  = (int16_t)(rawData[12] << 8 | rawData[13]);
}



void MPU6050::scaleData(const MPU6050_Data* raw, MPU6050_ScaledData* scaled) {


	// Datasheet: Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity) / 340 + 36.53


    // ±2g sensibility 16384.0f
    scaled->Accel_X = (float)raw->Accel_X / 16384.0f;
    scaled->Accel_Y = (float)raw->Accel_Y / 16384.0f;
    scaled->Accel_Z = (float)raw->Accel_Z / 16384.0f;

    scaled->Temp = ((float)raw->Temp / 340.0f) + 36.53f;

    // ±250 deg/s sensibility 131.0f
    scaled->Gyro_X = (float)raw->Gyro_X / 131.0f;
    scaled->Gyro_Y = (float)raw->Gyro_Y / 131.0f;
    scaled->Gyro_Z = (float)raw->Gyro_Z / 131.0f;
}

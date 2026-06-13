/*
 * mpu6050.h
 *
 *  Created on: Jun 7, 2026
 *      Author: yufur
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include <stdint.h>

#define MPU6050_ADDR    0x68 //
#define WHO_AM_I_REG    0x75
#define PWR_MGMT_1_REG  0x6B // Power menagment reg
#define ACCEL_XOUT_H_REG    0x3B

struct MPU6050_Data {
    int16_t Accel_X;
    int16_t Accel_Y;
    int16_t Accel_Z;
    int16_t Temp;
    int16_t Gyro_X;
    int16_t Gyro_Y;
    int16_t Gyro_Z;
};

struct MPU6050_ScaledData {
    float Accel_X;
    float Accel_Y;
    float Accel_Z;
    float Temp;
    float Gyro_X;
    float Gyro_Y;
    float Gyro_Z;
};

class MPU6050 {
public:
    static void init();
    static void readAll(MPU6050_Data* data);

    static void scaleData(const MPU6050_Data* raw, MPU6050_ScaledData* scaled);

};


#endif /* INC_MPU6050_H_ */

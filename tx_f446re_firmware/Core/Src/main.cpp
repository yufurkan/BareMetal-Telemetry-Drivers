#include "rcc_config.h"
#include "delay.h"
#include "i2c_driver.h"
#include "mpu6050.h"

#define MPU6050_ADDR 0x68
#define WHO_AM_I_REG 0x75

MPU6050_Data imu_data;
MPU6050_ScaledData scaled_imu;


int main(void) {



    SystemClock_Config();
    Delay::init();
    I2C::init();

    Delay::ms(50);

    volatile uint8_t mpu_id = 0;
    mpu_id = I2C::readByte(MPU6050_ADDR, WHO_AM_I_REG);

    MPU6050::init();
    while(1) {

    	MPU6050::readAll(&imu_data);
    	MPU6050::scaleData(&imu_data, &scaled_imu);
        Delay::ms(100);

    }
}

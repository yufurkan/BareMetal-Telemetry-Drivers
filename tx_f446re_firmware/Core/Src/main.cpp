#include "rcc_config.h"
#include "delay.h"
#include "i2c_driver.h"

#define MPU6050_ADDR 0x68
#define WHO_AM_I_REG 0x75


int main(void) {



    SystemClock_Config();
    Delay::init();
    I2C::init();


    volatile uint8_t mpu_id = 0;
    mpu_id = I2C::readByte(MPU6050_ADDR, WHO_AM_I_REG);


    while(1) {

        Delay::ms(1000);

    }
}

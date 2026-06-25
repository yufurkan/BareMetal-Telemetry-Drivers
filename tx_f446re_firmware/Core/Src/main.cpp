#include "rcc_config.h"
#include "delay.h"
#include "i2c_driver.h"
#include "mpu6050.h"
#include "nrf24l01.h"
#include "spi_driver.h"

#define MPU6050_ADDR 0x68
#define WHO_AM_I_REG 0x75

MPU6050_Data imu_data;
MPU6050_ScaledData scaled_imu;

uint8_t nrf24_test_val = 0;

int main(void) {



    SystemClock_Config();

    Delay::init();
    I2C::init();
    MPU6050::init();

    SPI::init();
    NRF24::init();

    Delay::ms(50);

    volatile uint8_t mpu_id = 0;
    mpu_id = I2C::readByte(MPU6050_ADDR, WHO_AM_I_REG);

    //Test nrlf24
    nrf24_test_val = NRF24::readReg(REG_RF_SETUP);


    uint8_t RF_CHANNEL=115;
    uint8_t tx_address[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
    bool tx_status = false;

    NRF24::tx_mode(tx_address, RF_CHANNEL);
    while(1) {

    	MPU6050::readAll(&imu_data);
    	MPU6050::scaleData(&imu_data, &scaled_imu);

    	tx_status = NRF24::transmit((uint8_t*)&scaled_imu, sizeof(scaled_imu));

        Delay::ms(100);

    }
}

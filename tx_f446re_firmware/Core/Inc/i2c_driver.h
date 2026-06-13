/*
 * i2c_driver.h
 *
 *  Created on: May 31, 2026
 *      Author: yufur
 */

#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_



#include <stdint.h>

class I2C {
public:

    static void init(void);

    static void writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);

    static uint8_t readByte(uint8_t devAddr, uint8_t regAddr);

    static void readBytes(uint8_t devAddr, uint8_t regAddr,uint8_t readCount, uint8_t* data);

private:
    I2C() = delete; // Static Class
};

#endif /* INC_I2C_DRIVER_H_ */

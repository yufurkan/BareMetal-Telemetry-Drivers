/*
 * spi_driver.h
 *
 *  Created on: Jun 14, 2026
 *      Author: yufur
 */

#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_



#include <stdint.h>

class SPI {
public:

    static void init(void);


    static void cs_enable(void);
    static void cs_disable(void);

    static uint8_t transmitReceive(uint8_t data);
};


#endif /* INC_SPI_DRIVER_H_ */

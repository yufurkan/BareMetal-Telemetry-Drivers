/*
 * i2c_driver.cpp
 *
 *  Created on: May 30, 2026
 *      Author: yufur
 */




#include "i2c_driver.h"
#include "stm32f446xx.h"


//Reference Manual 24.6 I2C Registers



void I2C::init(void) {

	// SDA=PB7  SCL=PB6

    RCC->APB1ENR |= (1 << 21); // I2C1 clock enable
    RCC->AHB1ENR |= (1 << 1);  // GPIOB clock enable


    // MODER = 10
    GPIOB->MODER &= ~((3 << 12) | (3 << 14));
    GPIOB->MODER |=  ((2 << 12) | (2 << 14)); // AF  set


    // Open-Drain
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);

    // High Speed OSPEEDR = 10
    GPIOB->OSPEEDR |= ((2 << 12) | (2 << 14));

    // Pull-up enable
    GPIOB->PUPDR &= ~((3 << 12) | (3 << 14));
    GPIOB->PUPDR |=  ((1 << 12) | (1 << 14));

    // Alternate Function 4 AF4
    //  24-27 (PB6) and  28-31 (PB7)
    GPIOB->AFR[0] |= (4 << 24) | (4 << 28);

    // I2C  Reset
    I2C1->CR1 |= (1 << 15);  // SWRST = 1
    I2C1->CR1 &= ~(1 << 15); // SWRST = 0

    // I2C Clock speed Configuration
    // APB1 45 MHz  180/4
    I2C1->CR2 = 45; // FREQ = 45 MHz

    // 100 kHz
    // Thigh = Tlow = 5000 ns. T_pclk1 = 1/45MHz = 22.2 ns. CCR = 5000 / 22.2 = ~225
    I2C1->CCR = 225;

    // Trise = FREQ + 1
    I2C1->TRISE = 46;

    // Start
    I2C1->CR1 |= (1 << 0); // PE = 1
}


 void I2C::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data){

	//mpu 6050 adress 68

	uint32_t timeout;

	    // Check if line is busy
	    timeout = 100000;
	    while(I2C1->SR2 & (1 << 1)) {
	        if(--timeout == 0) return; // Failsafe
	    }

	    // start signal
	    I2C1->CR1 |= (1 << 8);

	    //
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 0))) { if(--timeout == 0) return; }



	    //– Transmitter mode: Byte transmission starts automatically when a byte is written in the DR register. A continuous transmit stream can be maintained if the next data to be transmitted isput in DR once the transmission is started (TxE = 1)
	    //– Receiver mode: Received byte is copied into DR (RxNE = 1). A continuous transmit streamcan be maintained if DR is read before the next data byte is received (RxNE = 1).Note: In target mode, the address is not copied into DR. Write collision is not managed (DR
	    //can be written if TxE = 0). If an ARLO event occurs on ACK pulse, the received byte is
	    //not copied into DR and so cannot be read.

	    // Sending device adress data
	    I2C1->DR = (devAddr << 1);

	    // ADDR bite: Wait for address confirmation (ACK signal).
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 1))) { if(--timeout == 0) return; }


	    //Clearing Status Registres--------------->
	    //Reading I2C_SR2 after reading I2C_SR1 clears the ADDR flag, even if the ADDR flag was
	    //set after reading I2C_SR1. Consequently, I2C_SR2 must be read only when ADDR is found
	    //set in I2C_SR1 or when the STOPF bit is cleared.


	    //	volatile uint32_t clearFlag = I2C1->SR1; // Due to fact that In high optimizatinon levels comp can remove this line, uint32_t clearFlag line is volatile
	    //	clearFlag = I2C1->SR2;
	    //	(void)clearFlag; // cast to void prevents compiler warnings

	    (void)I2C1->SR1;
	    (void)I2C1->SR2;

	    //Clearing Status Registres---------------<


	    I2C1->DR = regAddr;


	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 7))) { if(--timeout == 0) return; }

	    // send main data
	    I2C1->DR = data;

	    // BTF (byte transfer finished flag )- SR1 2. bite
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 2))) { if(--timeout == 0) return; }

	    // STOP signal
	    I2C1->CR1 |= (1 << 9);


}

uint8_t I2C::readByte(uint8_t devAddr, uint8_t regAddr){


	uint32_t timeout;
	    uint8_t receivedData = 0;

	    // Check if line is busy
	    timeout = 100000;
	    while(I2C1->SR2 & (1 << 1)) { if(--timeout == 0) return 0; }

	    // START signal
	    I2C1->CR1 |= (1 << 8);
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 0))) { if(--timeout == 0) return 0; }

	    // Sending device adress data
	    I2C1->DR = (devAddr << 1);
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 1))) { if(--timeout == 0) return 0; }

	    //clear tatus registers
	    (void)I2C1->SR1;
	   	(void)I2C1->SR2;

	    // Send register adress you want to read
	    I2C1->DR = regAddr;
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 7))) { if(--timeout == 0) return 0; }

	    // Repeated Start for entering reading mode
	    I2C1->CR1 |= (1 << 8);
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 0))) { if(--timeout == 0) return 0; }

	    // Send device adress agin but reading mode
	    I2C1->DR = (devAddr << 1) | 1;  // read signal

	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 1))) { if(--timeout == 0) return 0; }

	    // NACK
	    I2C1->CR1 &= ~(1 << 10);

	    // ADDR bayragini temizle
	    (void)I2C1->SR1;
	    (void)I2C1->SR2;

	    // STOP after reading finished
	    I2C1->CR1 |= (1 << 9);

	    // 7.Wait for the data to arrive Data Register
	    timeout = 100000;
	    while(!(I2C1->SR1 & (1 << 6))) { if(--timeout == 0) return 0; }

	   // Read
	    receivedData = I2C1->DR;

	    return receivedData;
}

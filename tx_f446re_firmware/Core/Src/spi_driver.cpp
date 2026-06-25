/*
 * spi_driver.cpp
 *
 *  Created on: Jun 14, 2026
 *      Author: yufur
 */




#include "spi_driver.h"
#include "stm32f446xx.h"

// Reference Manual 26.7 SPI Registers

void SPI::init(void) {

    // CS=PA4  SCK=PA5  MISO=PA6  MOSI=PA7 CE=PA3

    RCC->APB2ENR |= (1 << 12); // SPI1 clock enable
    RCC->AHB1ENR |= (1 << 0);  // GPIOA clock enable



    // --- GPIO Configuration ---

    // PA4: General Purpose Output MODER = 01
    GPIOA->MODER &= ~(3 << 8);
    GPIOA->MODER |=  (1 << 8);

    // PA5, PA6, PA7: Alternate Function MODER = 10
    GPIOA->MODER &= ~((3 << 10) | (3 << 12) | (3 << 14));
    GPIOA->MODER |=  ((2 << 10) | (2 << 12) | (2 << 14));

    // High Speed OSPEEDR = 11
    GPIOA->OSPEEDR |= ((3 << 10) | (3 << 12) | (3 << 14));

    // Alternate Function 5 (AF5) for SPI1
    GPIOA->AFR[0] &= ~((15 << 20) | (15 << 24) | (15 << 28));
    GPIOA->AFR[0] |=  ((5 << 20)  | (5 << 24)  | (5 << 28));

    // Default CS state: HIGH
    GPIOA->ODR |= (1 << 4);


    // --- GPIO Configuration ---/

    // --- SPI Configuration ---

    // NNS manual
    //SSM = 1, SSI = 1
    SPI1->CR1 |= (1 << 9) | (1 << 8);

    // Baud rate
    // APB2 is 90 MHz. BR[2:0]=011 | 90/16=5.625 MHz
    SPI1->CR1 |= (3 << 3);

    // Master selection
    SPI1->CR1 |= (1 << 2);//we are master

    // CPOL=0, CPHA=0, MSB First default

    // Enable SPI (SPE = 1)
    SPI1->CR1 |= (1 << 6);

    // --- SPI Configuration ---/
}

void SPI::cs_enable(void) {
    // Pull CS low to select device
    GPIOA->ODR &= ~(1 << 4);
}

void SPI::cs_disable(void) {
    // Pull CS high to deselect device
    GPIOA->ODR |= (1 << 4);
}

uint8_t SPI::transmitReceive(uint8_t data) {

	uint32_t timeout = 10000;
    // Wait until TX buffer is empty TXE = 1
    while (!(SPI1->SR & (1 << 1))){if (--timeout == 0) return 0x00;};

    // Send data
    SPI1->DR = data;


    // Wait until RX buffer is not empty RXNE = 1
    timeout = 10000;
    while (!(SPI1->SR & (1 << 0))){if (--timeout == 0) return 0x00;};

    // Read received data
    // When DR is read, hardware sets SPI1->SR as 0
    return SPI1->DR;
}

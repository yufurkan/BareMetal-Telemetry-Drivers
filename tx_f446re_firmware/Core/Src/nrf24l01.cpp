/*
 * nrf24l01.cpp
 *
 *  Created on: Jun 17, 2026
 *      Author: yufur
 */


#include <stdint.h>

#include "nrf24l01.h"
#include "spi_driver.h"
#include "stm32f446xx.h"

void NRF24::init(void) {
    // Configure CE Pin MODER = 01
    // This pin controls the RX/TX state of the NRF24 module
    GPIOA->MODER &= ~(3 << 6);
    GPIOA->MODER |=  (1 << 6);

    // Set CE low by default (Standby mode)
    ce_low();
}

void NRF24::ce_high(void) {
    GPIOA->ODR |= (1 << 3);
}

void NRF24::ce_low(void) {
    GPIOA->ODR &= ~(1 << 3);
}

void NRF24::writeReg(uint8_t reg, uint8_t data) {
    // Pull CS low to select the module
    SPI::cs_enable();

    //Send the write command ORed with the register address
    SPI::transmitReceive(CMD_W_REGISTER | reg);

    // send the data to be written
    SPI::transmitReceive(data);

    // Pull CS high to deselect
    SPI::cs_disable();
}

uint8_t NRF24::readReg(uint8_t reg) {

    SPI::cs_enable();


    // Note: CMD_R_REGISTER is 0x00, so we just send the register address
    SPI::transmitReceive(CMD_R_REGISTER | reg);

    // Send a dummy byte (0xFF) to generate clock pulses and read the response via MISO
    uint8_t val = SPI::transmitReceive(0xFF);

    // Pull CS high to deselect
    SPI::cs_disable();

    return val;
}

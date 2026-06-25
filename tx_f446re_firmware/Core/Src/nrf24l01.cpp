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

	// CE=PA3
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

    // Send a  byte 0xFF to generate clock pulses and read the response via MISO
    uint8_t val = SPI::transmitReceive(0xFF);

    // Pull CS high to deselect
    SPI::cs_disable();

    return val;
}

void NRF24::writeRegMulti(uint8_t reg, uint8_t *data, uint8_t size) {

    SPI::cs_enable();
    SPI::transmitReceive(CMD_W_REGISTER | reg);
    for (uint8_t i = 0; i < size; i++) {
        SPI::transmitReceive(data[i]);
    }
    SPI::cs_disable();
}




//------------------------------------------------------------------------------------------


void NRF24::tx_mode(uint8_t *address, uint8_t channel) {

    ce_low(); // standby mode

    //  Set RF Channel
    writeReg(REG_RF_CH, channel);

    //  Set TX and RX_P0 address to the same value 5 bytes
    writeRegMulti(REG_TX_ADDR, address, 5);
    writeRegMulti(REG_RX_ADDR_P0, address, 5);

    // Disable Auto-Acknowledgment for telemetry broadcasting
    writeReg(REG_EN_AA, 0x00);

    //  Disable Auto Retransmission
    writeReg(REG_SETUP_RETR, 0x00);

    //  Set RF Data Rate to 2Mbps, 0dBm output power
    writeReg(REG_RF_SETUP, 0x0E);

    // Config Reg: Power UP (Bit 1), TX Mode (Bit 0 = 0), Enable CRC (Bit 3)
    writeReg(REG_CONFIG, 0x0E);

    // Wait for NRF24 to power up
    Delay::ms(3);
}


bool NRF24::transmit(uint8_t *payload, uint8_t size) {
    // Flush TX FIFO
    SPI::cs_enable();
    SPI::transmitReceive(CMD_FLUSH_TX);
    SPI::cs_disable();

    // Clear STATUS register flags (Write 1 to clear Bit 4, 5, 6)
    writeReg(REG_STATUS, 0x70);

    //  Write payload to TX FIFO
    SPI::cs_enable();
    SPI::transmitReceive(CMD_W_TX_PAYLOAD);
    for (uint8_t i = 0; i < size; i++) {
        SPI::transmitReceive(payload[i]);
    }
    SPI::cs_disable();

    //Pulse CE pin to transmit
    ce_high();
    for (volatile int i = 0; i < 200; i++);
    ce_low();

    // Poll STATUS register until TX_DS (Bit 5) or MAX_RT (Bit 4) flag is set


    uint32_t timeout = 10000;
	uint8_t status;
	do {
		status = readReg(REG_STATUS);
		if (--timeout == 0) return false;
	} while (!(status & (1 << 5)) && !(status & (1 << 4)));

    //  Clear flags again
    writeReg(REG_STATUS, 0x70);

    // Return true if TX_DS Data Sent flag is set
    if (status & (1 << 5)) {
        return true;
    }
    return false;
}

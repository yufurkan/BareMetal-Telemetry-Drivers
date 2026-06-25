/*
 * nrf24l01.h
 *
 *  Created on: Jun 17, 2026
 *      Author: yufur
 */

#ifndef INC_NRF24L01_H_
#define INC_NRF24L01_H_

// --- Instruction Set
#define CMD_R_REGISTER    0x00  // Read register
#define CMD_W_REGISTER    0x20  // Write register
#define CMD_R_RX_PAYLOAD  0x61  // read incoming value payload
#define CMD_W_TX_PAYLOAD  0xA0  // write recive data
#define CMD_FLUSH_TX      0xE1  // clear tX buffer
#define CMD_FLUSH_RX      0xE2  // clear rx buffer
#define CMD_NOP           0xFF  // nop

// --- NRF24L01 Register Map
#define REG_CONFIG        0x00  // config register
#define REG_EN_AA         0x01  // Auto Acknowledgment
#define REG_EN_RXADDR     0x02  // enable rx addr pipes
#define REG_SETUP_AW      0x03  // addr width range
#define REG_SETUP_RETR    0x04  // auto retransmit
#define REG_RF_CH         0x05  // RF channel
#define REG_RF_SETUP      0x06  // rf speed- pawer
#define REG_STATUS        0x07  // status reg (Interrupt flags)
#define REG_TX_ADDR       0x10  // tx target addr
// --- NRF24L01 Register Map/

class NRF24 {
public:
    static void init(void);


    static void writeReg(uint8_t reg, uint8_t data);
    static uint8_t readReg(uint8_t reg);
    static void writeRegMulti(uint8_t reg, uint8_t *data, uint8_t size);
    // CE PA3
    static void ce_high(void);
    static void ce_low(void);

    //tx
    static void tx_mode(uint8_t *address, uint8_t channel);
    static bool transmit(uint8_t *payload, uint8_t size);
};


#endif /* INC_NRF24L01_H_ */

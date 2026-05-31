/*
 * delay.cpp
 *
 *  Created on: May 31, 2026
 *      Author: yufur
 */



#include "delay.h"
#include "stm32f446xx.h"

void Delay::init(void) {

    // Stop SysTick counter and clean
    SysTick->CTRL = 0;
    SysTick->VAL = 0;

    // Launch load value for 1ms
    // 179999 fits  24-bit  (16.777.215)
    SysTick->LOAD = 180000 - 1;

    // Start SysTick
    // Bite 2 (CLKSOURCE): 1 = cpu clock
    // Bit 0 (ENABLE): 1 = Start counter

    SysTick->CTRL |= (1 << 2) | (1 << 0);
}

void Delay::ms(uint32_t delay_ms) {

    for (uint32_t i = 0; i < delay_ms; i++) {

        SysTick->VAL = 0;
        while (!(SysTick->CTRL & (1 << 16)));
    }
}

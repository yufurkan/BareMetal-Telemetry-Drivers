/*
 * rcc_config.cpp
 *
 *  Created on: May 28, 2026
 *      Author: yufur
 */

#include "rcc_config.h"
#include "stm32f446xx.h"

#define HARDWARE_TIMEOUT 10000U

void System_ErrorHandler(void) {

	NVIC_SystemReset();
    while(1);
}


void SystemClock_Config(void) {

	RCC->APB1ENR |= (1 << 28);//enable PWR clock
	PWR->CR |= (3 << 14);

	// M=8 N=180 P=2

	//RCC->PLLCFGR |= (16 << 0);  //P=00 = 2 Default
	RCC->PLLCFGR |= (8 << 0);//PLLM
	RCC->PLLCFGR |= (180 << 6);//PLLN


	//to 180 mhz from 166
	RCC->CR |=(1<<24);

	timeout_counter = 0;
	while(!(RCC->CR & (1<<25))) {if(++timeout_counter > HARDWARE_TIMEOUT) System_ErrorHandler();}

	timeout_counter = 0;
	PWR->CR |= (1<<16);
	while(!(PWR->CSR & (1U<<16))){if(++timeout_counter > HARDWARE_TIMEOUT) System_ErrorHandler();}


	PWR->CR |= (1<<17); // Over-drive switching enabled

	// Wait ODSWRDY
	timeout_counter = 0;
	while(!(PWR->CSR & (1U<<17))){if(++timeout_counter > HARDWARE_TIMEOUT) System_ErrorHandler();}


	FLASH->ACR |= (5 << 0);


	// APB1 45MHZ
	RCC->CFGR |= (5 << 10);

	// APB2 MHZ
	RCC->CFGR |= (4 << 13);


	//sw=10 system clock ppl
	RCC->CFGR |= (2 << 0);
	timeout_counter = 0;
	while((RCC->CFGR & (3 << 2)) != (2 << 2)){if(++timeout_counter > HARDWARE_TIMEOUT) System_ErrorHandler();}

}



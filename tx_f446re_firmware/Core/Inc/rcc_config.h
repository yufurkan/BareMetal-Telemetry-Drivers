/*
 * rcc_config.h
 *
 *  Created on: May 28, 2026
 *      Author: yufur
 */

#ifndef INC_RCC_CONFIG_H_
#define INC_RCC_CONFIG_H_

#include <stdint.h>


#define HARDWARE_TIMEOUT 10000U

void SystemClock_Config(void);
void System_ErrorHandler(void);

#endif /* INC_RCC_CONFIG_H_ */

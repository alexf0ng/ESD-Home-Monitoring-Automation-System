/*
 * gpio.h
 *
 *  Created on: Aug 22, 2026.
 *      Author: user
 *
 *      Description: to initialize gpio pin used
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

typedef struct {
	uint16_t pin;
} Gpio;

void GPIO_obLED_init(Gpio *gpio);
void GPIO_btn_init(Gpio *gpio);
void GPIO_openObLED(bool pg13, bool pg14);

#endif /* GPIO_GPIO_H_ */

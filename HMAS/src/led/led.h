/*
 * led.h
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */

#ifndef LED_LED_H_
#define LED_LED_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
	uint32_t clock;
	GPIOPuPd_TypeDef pull;
	GPIOSpeed_TypeDef speed;
	GPIOOType_TypeDef type;
} LedGpio;

void led_Init(LedGpio *ledgpio);
void open_led(LedGpio *ledgpio);
void close_led(LedGpio *ledgpio);
void open_all_led();
void close_all_led();

#endif /* LED_LED_H_ */

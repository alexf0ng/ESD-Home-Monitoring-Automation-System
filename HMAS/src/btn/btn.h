/*
 * btn.h
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */

#ifndef BTN_BTN_H_
#define BTN_BTN_H_
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
	uint32_t clock;
	GPIOPuPd_TypeDef pull;
} BtnGpio;

void extbtn_Init(BtnGpio* btngpio);




#endif /* BTN_BTN_H_ */

/*
 * sw420.h
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */

#ifndef SENSOR_SW420_SW420_H_
#define SENSOR_SW420_SW420_H_

#include "ui.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>
#include "usart/usart.h"
#include "atb3972/atb3972.h"

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
	uint32_t clock;
	GPIOPuPd_TypeDef pull;
}Sw420Gpio;

void sw420_Init(Sw420Gpio *sw420gpio);
void sw420_listen(Sw420Gpio *sw420gpio, Atb3972 *atb3972);

#endif /* SENSOR_SW420_SW420_H_ */

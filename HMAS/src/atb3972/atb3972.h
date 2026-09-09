/*
 * atb3972.h
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */
#ifndef __ATB3972_H
#define __ATB3972_H

#include "stm32f4xx.h"
#include "lvgl.h"

typedef struct {
	uint32_t clock_gpio;
	uint32_t clock_dac;
	uint32_t clock_tim;
	uint16_t pin;
	GPIOMode_TypeDef mode;
	GPIOPuPd_TypeDef pull;
	GPIO_TypeDef *port;

	TIM_TypeDef *tim;
	uint16_t tim_period;
	uint16_t tim_prescaler;
	uint16_t tim_clockdivision;
	uint16_t tim_countermode;

	uint32_t dac_channel;
	uint32_t dac_trigger;
	uint32_t dac_wave;
	uint32_t dac_triangle_amplitude;
	uint32_t dac_outputbuffer;
} Atb3972;

void atb_Init(Atb3972 *atb);
void atb_start(Atb3972 *atb);
void atb_stop(Atb3972 *atb);
void atb_play(Atb3972 *atb);

#endif

/*
 * led.c
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */

#include "led.h"

static LedGpio *led_array[10];
static uint8_t led_count = 0;

void led_Init(LedGpio *ledgpio){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(ledgpio->clock, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ledgpio->pin;
	GPIO_InitStructure.GPIO_Mode = ledgpio->mode;
	GPIO_InitStructure.GPIO_OType = ledgpio->type;
	GPIO_InitStructure.GPIO_Speed = ledgpio->speed;
	GPIO_InitStructure.GPIO_PuPd = ledgpio->pull;

	GPIO_Init(ledgpio->port, &GPIO_InitStructure);

	if(led_count < 10) led_array[led_count++] = ledgpio;
}

void open_led(LedGpio *ledgpio){
	GPIO_SetBits(ledgpio->port, ledgpio->pin);
}

void close_led(LedGpio *ledgpio){
	GPIO_ResetBits(ledgpio->port, ledgpio->pin);
}

void open_all_led(){
	for (uint8_t i = 0; i < led_count; i++)
		open_led(led_array[i]);
}

void close_all_led(){
	for (uint8_t i = 0; i < led_count; i++)
		close_led(led_array[i]);
}


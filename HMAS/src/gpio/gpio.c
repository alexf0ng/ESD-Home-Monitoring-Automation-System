/*
 * gpio.h
 *
 *  Created on: Aug 22, 2026
 *      Author: user
 */
#include "gpio.h"

void GPIO_obLED_init(Gpio *gpio)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(
		RCC_AHB1Periph_GPIOG,
		ENABLE
	);

    GPIO_InitStructure.GPIO_Pin = gpio->pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_Init(
		GPIOG,
		&GPIO_InitStructure
	);
}

void GPIO_btn_init(Gpio *gpio)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(
		RCC_AHB1Periph_GPIOC,
		ENABLE
	);

    GPIO_InitStructure.GPIO_Pin = gpio->pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    GPIO_Init(
		GPIOC,
		&GPIO_InitStructure
	);
}

void GPIO_openObLED(bool pg13, bool pg14){
	if(pg13)
		GPIO_SetBits(GPIOG, GPIO_Pin_13);
	else
		GPIO_ResetBits(GPIOG, GPIO_Pin_13);

	if(pg14)
		GPIO_SetBits(GPIOG, GPIO_Pin_14);
	else
		GPIO_ResetBits(GPIOG, GPIO_Pin_14);
}

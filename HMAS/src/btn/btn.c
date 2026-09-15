/*
 * btn.c
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */

#include "btn.h"

void extbtn_Init(BtnGpio* btngpio){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(btngpio->clock ,ENABLE);

	GPIO_InitStructure.GPIO_Pin = btngpio->pin;
	GPIO_InitStructure.GPIO_Mode = btngpio->mode;
	GPIO_InitStructure.GPIO_PuPd = btngpio->pull;

	GPIO_Init(btngpio->port, &GPIO_InitStructure);
}



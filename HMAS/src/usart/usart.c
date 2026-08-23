/*
 * usart.c
 *
 *  Created on: Aug 22, 2026
 *      Author: user
 */
#include "usart.h"

void USART1_init(Usart1 *usart1) {
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	RCC_AHB1PeriphClockCmd(
		RCC_AHB1Periph_GPIOA,
		ENABLE
	);
	RCC_APB2PeriphClockCmd(
		RCC_APB2Periph_USART1,
		ENABLE
	);

	GPIO_InitStruct.GPIO_Pin = usart1->tx_pin | usart1->rx_pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(
		GPIOA,
		&GPIO_InitStruct
	);

	GPIO_PinAFConfig(
		usart1->port,
		usart1->tx_source,
		GPIO_AF_USART1
	);
	GPIO_PinAFConfig(
		usart1->port,
		usart1->rx_source,
		GPIO_AF_USART1
	);

	USART_InitStruct.USART_BaudRate = usart1->baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(
		USART1,
		&USART_InitStruct
	);

	USART_Cmd(
		USART1,
		ENABLE
	);
}

void USART1_send_string(const char *str){
	while(*str){
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, *str++);
	}
}

/*
 * usart.h
 *
 *  Created on: Aug 22, 2026
 *      Author: user
 *
 *      Description: for usart send to com port
 *
 */

#ifndef USART_USART_H_
#define USART_USART_H_

#include <stdint.h>
#include "stm32f4xx.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t tx_pin;
    uint16_t rx_pin;
    uint8_t tx_source;
    uint8_t rx_source;
    uint32_t baudrate;
} Usart1;

void USART1_init(Usart1 *usart1);
void USART1_send_string(const char *str);

#endif /* USART_USART_H_ */

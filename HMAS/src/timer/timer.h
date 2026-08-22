/*
 * timer.h
 *
 *  Created on: Aug 22, 2026
 *      Author: user
 */

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

#include <stdint.h>
#include "stm32f4xx.h"

typedef struct
{
    TIM_TypeDef *timer;
    uint32_t timer_clock;
    uint16_t prescaler;
    uint32_t period;
    IRQn_Type irq_channel;
    uint8_t preemption_priority;
    uint8_t sub_priority;

} Timer;

void timer_init(Timer *timer);
void timer_NVIC_init(Timer *timer);



#endif /* TIMER_TIMER_H_ */

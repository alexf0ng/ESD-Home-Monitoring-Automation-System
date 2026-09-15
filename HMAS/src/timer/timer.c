/*
 * timier.c
 *
 *  Created on: Aug 22, 2026
 *      Author: user
 *
 *      Description: for timer to count delay
 */
#include "timer.h"

void timer_init(Timer *timer)
{
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;

    RCC_APB1PeriphClockCmd(
		timer->timer_clock,
        ENABLE
    );
    TIM_BaseStruct.TIM_Prescaler = timer->prescaler;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = timer->period;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(
        timer->timer,
        &TIM_BaseStruct
    );

    timer->enable_interrupt ? TIM_ITConfig(
									timer->timer,
									TIM_IT_Update,
									ENABLE
								) : "";
}

void timer_NVIC_init(Timer *timer)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = timer->irq_channel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = timer->preemption_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = timer->sub_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}


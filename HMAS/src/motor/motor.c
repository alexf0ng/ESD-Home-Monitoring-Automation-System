#include "motor.h"
#include "usart/usart.h"
#include <stdio.h>
extern char buffer[50];

static uint8_t motor_duty = 0;

void motor_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    TIM_OCInitTypeDef TIM_OCStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // ENB on PB7 = TIM4_CH2
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TIM4 ticks at 90MHz (APB1 45MHz x2), period = 90000000 / 50000 - 1
    TIM_BaseStruct.TIM_Prescaler         = 0;
    TIM_BaseStruct.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period            = 1799;    // 50kHz
    TIM_BaseStruct.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);

    // PWM mode 2 + active low polarity, same as Lab 2.
    // the two inversions cancel so the pin is HIGH while CNT < CCR,
    // which is what an active high ENB wants. start stopped (CCR 0).
    TIM_OCStruct.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OCStruct.TIM_Pulse       = 0;
    TIM_OC2Init(TIM4, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_Cmd(TIM4, ENABLE);

    motor_duty = 0;
}

void motor_set_duty(uint8_t duty_percent){
    uint32_t pulse;

    if (duty_percent > 100)
        duty_percent = 100;

    motor_duty = duty_percent;

    pulse = ((1799 + 1) * duty_percent) / 100;

    TIM_SetCompare2(TIM4, pulse);

    sprintf(buffer, "motor duty = %d%% ccr = %d \r\n", (int)duty_percent, (int)pulse);
    USART1_send_string(buffer);
}

void motor_on(void){
    motor_set_duty(100);     // spec: 85% duty cycle at 50kHz
}

void motor_off(void){
    motor_set_duty(0);
}

bool motor_is_on(void){
    return (motor_duty > 0);
}

uint8_t motor_get_duty(void){
    return motor_duty;
}

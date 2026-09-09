/*
 * atb3972.c
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */
/*
 * analog_test_board.c
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */
#include "atb3972.h"

void atb_Init(Atb3972 *atb)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    DAC_InitTypeDef DAC_InitStructure;

    RCC_AHB1PeriphClockCmd(atb->clock_gpio, ENABLE);
    RCC_APB1PeriphClockCmd(atb->clock_dac, ENABLE);
    RCC_APB1PeriphClockCmd(atb->clock_tim, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = atb->pin;
    GPIO_InitStructure.GPIO_Mode = atb->mode;
    GPIO_InitStructure.GPIO_PuPd = atb->pull;
    GPIO_Init(atb->port, &GPIO_InitStructure);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period        = atb->tim_period;
    TIM_TimeBaseStructure.TIM_Prescaler     = atb->tim_prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = atb->tim_clockdivision;
    TIM_TimeBaseStructure.TIM_CounterMode   = atb->tim_countermode;
    TIM_TimeBaseInit(atb->tim, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(atb->tim, TIM_TRGOSource_Update);

    DAC_InitStructure.DAC_Trigger = atb->dac_trigger;
    DAC_InitStructure.DAC_WaveGeneration = atb->dac_wave;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = atb->dac_triangle_amplitude;
    DAC_InitStructure.DAC_OutputBuffer = atb->dac_outputbuffer;
    DAC_Init(atb->dac_channel, &DAC_InitStructure);

    if (atb->dac_channel == DAC_Channel_1)
        DAC_SetChannel1Data(DAC_Align_12b_R, 0);
    else
        DAC_SetChannel2Data(DAC_Align_12b_R, 0);

    // disable, only start when start call
    TIM_Cmd(atb->tim, DISABLE);
    DAC_Cmd(atb->dac_channel, DISABLE);
}

void atb_start(Atb3972 *atb)
{
    TIM_Cmd(atb->tim, ENABLE);
    DAC_Cmd(atb->dac_channel, ENABLE);
}

void atb_stop(Atb3972 *atb)
{
    DAC_Cmd(atb->dac_channel, DISABLE);
    TIM_Cmd(atb->tim, DISABLE);
}

// Sets the timer's auto-reload value, which controls the triangle wave's pitch.
// Lower period = faster stepping = higher pitch.
static void atb_set_period(Atb3972 *atb, uint16_t period){
    atb->tim->ARR = period;
}

// Crude blocking delay (busy loop). Fine for a short one-shot jingle;
// avoid calling AnalogBoard_PlayJingle() from a fast-repeating LVGL timer.
static void atb_delayms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 4000; i++) { __NOP(); }
}

// An original short rising fanfare - four ascending notes, our own composition.
void atb_play(Atb3972 *atb){
    uint16_t notePeriods[] = { 3000, 2200, 1600, 1000 };
    uint16_t noteDurationMs = 120;

    atb_start(atb);

    for (int i = 0; i < 4; i++)
    {
        atb_set_period(atb, notePeriods[i]);
        atb_delayms(noteDurationMs);
    }

    atb_stop(atb);
}


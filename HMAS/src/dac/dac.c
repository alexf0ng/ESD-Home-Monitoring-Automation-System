/*
 * dac.c
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */

#include "dac.h"

void Dac_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    DAC_InitTypeDef DAC_InitStructure;

    // --- Clocks ---
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    // --- PA4 as analog (DAC_OUT1) ---
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // --- TIM6 timebase: drives the DAC's automatic triangle-wave stepping ---
    // Values reused from the vendor DAC test sample (period=5000, prescaler=83)
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period        = 5000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler     = 83;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    // TIM6 update event becomes the DAC's trigger source
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

    // --- DAC channel 1: hardware-generated triangle wave, stepped by TIM6 ---
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    // Base/offset value the triangle wave rides on top of
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);

    // Leave both stopped until DacTone_Start() is called
    TIM_Cmd(TIM6, DISABLE);
    DAC_Cmd(DAC_Channel_1, DISABLE);
}

void dac_tone_start(void)
{
    TIM_Cmd(TIM6, ENABLE);
    DAC_Cmd(DAC_Channel_1, ENABLE);
}

void dac_tone_stop(void)
{
    DAC_Cmd(DAC_Channel_1, DISABLE);
    TIM_Cmd(TIM6, DISABLE);
}

// Sets TIM6's auto-reload value, which controls the DAC triangle wave's pitch.
// Lower period = faster stepping = higher pitch.
static void dac_tone_set_period(uint16_t period)
{
    TIM6->ARR = period;
}

// Crude blocking delay (busy loop). Fine for a short one-shot jingle;
// avoid calling DacTone_PlayJingle() from a fast-repeating LVGL timer,
// since it will briefly block lv_timer_handler() while playing.
static void dac_tone_delayms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 4000; i++) { __NOP(); }
}

// An original short rising fanfare - four ascending notes, our own composition,
// not derived from any existing melody.
void dac_tone_play_jingle(void)
{
    uint16_t notePeriods[] = { 3000, 2200, 1600, 1000 };
    uint16_t noteDurationMs = 120;

    DacTone_Start();

    for (int i = 0; i < 4; i++)
    {
        DacTone_SetPeriod(notePeriods[i]);
        DacTone_DelayMs(noteDurationMs);
    }

    DacTone_Stop();
}

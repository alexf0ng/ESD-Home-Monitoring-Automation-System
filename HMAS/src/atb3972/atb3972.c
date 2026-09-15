/*
 * atb3972.c
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */
#include "atb3972.h"

#define ATB_JINGLE_NOTE_COUNT   2
#define ATB_JINGLE_NOTE_MS      180

static const uint16_t atb_jingle_notes[ATB_JINGLE_NOTE_COUNT] = {  14, 10 };

static Atb3972    *atb_jingle_target  = NULL;
static uint8_t      atb_jingle_index   = 0;
static lv_timer_t  *atb_jingle_timer   = NULL;


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
static void atb_set_period(Atb3972 *atb, uint16_t period){
    atb->tim->ARR = period;
}

static void atb_jingle_step(lv_timer_t *timer)
{
    if (atb_jingle_index >= ATB_JINGLE_NOTE_COUNT)
    {
        // finished all notes, stop the tone and clean up
        atb_stop(atb_jingle_target);
        lv_timer_del(timer);
        atb_jingle_timer = NULL;
        return;
    }

    atb_set_period(atb_jingle_target, atb_jingle_notes[atb_jingle_index]);
    atb_jingle_index++;
}

// Starts the jingle. Returns immediately - playback continues in the
// background via an internal lv_timer, safe to call again once finished
// if called again while already playing, the previous jingle is cancelled
// and restarted from the first note.
void atb_play(Atb3972 *atb)
{
    if (atb_jingle_timer != NULL)
    {
        lv_timer_del(atb_jingle_timer);
        atb_jingle_timer = NULL;
    }

    atb_jingle_target = atb;
    atb_jingle_index   = 0;

    atb_start(atb);
    atb_set_period(atb, atb_jingle_notes[0]);
    atb_jingle_index = 1;

    atb_jingle_timer = lv_timer_create(atb_jingle_step, ATB_JINGLE_NOTE_MS, NULL);
}

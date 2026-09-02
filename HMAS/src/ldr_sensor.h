#ifndef __LDR_SENSOR_H
#define __LDR_SENSOR_H

#include "stm32f4xx.h"

void LDR_Init(void);

uint16_t LDR_ReadRaw(void);

float LDR_ReadIntensityPercent(void);

#endif 

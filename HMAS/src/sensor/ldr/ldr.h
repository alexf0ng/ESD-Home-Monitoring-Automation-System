/*
 * ldr.h
 *
 *  Created on: Sep 8, 2026
 *      Author: User
 */

#ifndef SENSOR_LDR_LDR_H_
#define SENSOR_LDR_LDR_H_
#include "stm32f4xx.h"
void LDR_Init(void);
float LDR_ReadIntensityPercent(void);


#endif /* SENSOR_LDR_LDR_H_ */

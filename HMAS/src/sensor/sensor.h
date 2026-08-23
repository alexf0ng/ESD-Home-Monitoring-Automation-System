/*
 * sensor.h
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#ifndef SENSOR_SENSOR_H_
#define SENSOR_SENSOR_H_

#include "usart/usart.h"
#include "dht22/dht22.h"
#include "usart/usart.h"
#include <stdio.h>
#include <stddef.h>

typedef struct{
	float temp;
	float hum;
	float ldr;
	const char *excmsgDht22;
	const char *excmsgLdr;
} Sensor;

typedef struct{
	float temp;
	float hum;
} Dht22;

typedef struct{
	float ldr;
} Ldr;

Dht22 temp_hum(void);
Ldr light_dependent_resistor(void);
Sensor temp_hum_ldr(void); // fetch all 3 sensor val

#endif /* SENSOR_SENSOR_H_ */

#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t duty;   // current duty percent (0-100); sensor_page.c reads this directly
} Motor;

void    motor_Init(void);
void    motor_set_duty(Motor *motor, uint8_t duty_percent);
void    motor_on(Motor *motor);
void    motor_off(Motor *motor);
bool    motor_is_on(Motor *motor);
uint8_t motor_get_duty(Motor *motor);

#endif /* MOTOR_MOTOR_H_ */

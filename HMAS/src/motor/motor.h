#ifndef MOTOR_MOTOR_H_
#define MOTOR_MOTOR_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

void    motor_Init(void);
void    motor_set_duty(uint8_t duty_percent);
void    motor_on(void);
void    motor_off(void);
bool    motor_is_on(void);
uint8_t motor_get_duty(void);

#endif /* MOTOR_MOTOR_H_ */

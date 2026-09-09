/*
 * sw420.c
 *
 *  Created on: Sep 9, 2026
 *      Author: User
 */
#include "sw420.h"
#define MOTION_DEBOUNCE_MS      50   // how long the signal must hold steady to be trusted
#define MOTION_CHECK_INTERVAL   20   // expected polling interval in ms

static bool motion_stable_state = false;
static bool motion_last_raw = false;
static uint32_t motion_stable_ms = 0;
static Atb3972 *atb = NULL;
extern char buffer[50];

void sw420_Init(Sw420Gpio *sw420gpio){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(sw420gpio->clock, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = sw420gpio->pin;
	GPIO_InitStructure.GPIO_Mode = sw420gpio->mode;
	GPIO_InitStructure.GPIO_PuPd = sw420gpio->pull;  // module actively drives DO itself
	GPIO_Init(sw420gpio->port, &GPIO_InitStructure);

	motion_stable_state = false;
	motion_last_raw     = false;
	motion_stable_ms    = 0;
}

bool sw420_read(Sw420Gpio *sw420gpio) {
	return (GPIO_ReadInputDataBit(sw420gpio->port, sw420gpio->pin) == SET);
}
bool sw420_is_detected(Sw420Gpio *sw420gpio){
	bool raw = sw420_read(sw420gpio);
	if (raw != motion_last_raw){
		// signal just changed (or is bouncing)
		motion_last_raw  = raw;
		motion_stable_ms = 0;
	}
	else{
		motion_stable_ms += MOTION_CHECK_INTERVAL;

		if (motion_stable_ms >= MOTION_DEBOUNCE_MS){
			motion_stable_state = raw;  // held steady long enough - accept it
		}
	}

	return motion_stable_state;
}

static void sw420_check(lv_timer_t *timer){
	Sw420Gpio *gpio = (Sw420Gpio *)lv_timer_get_user_data(timer);
	static bool tonePlaying = false;

	if (sw420_is_detected(gpio)){
		if (!tonePlaying){
			sprintf(buffer, "Motion detected!\r\n");
			USART1_send_string(buffer);
			atb_start(atb);
			tonePlaying = true;
		}
	}
	else{
		if (tonePlaying){
			atb_stop(atb);
			tonePlaying = false;
		}
	}
}
void sw420_listen(Sw420Gpio *sw420gpio, Atb3972 *atb3972){
	atb = atb3972;
    lv_timer_create(sw420_check, MOTION_CHECK_INTERVAL, sw420gpio);
}



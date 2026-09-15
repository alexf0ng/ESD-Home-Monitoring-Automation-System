/*
 * setting_page.h
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#ifndef PAGE_SETTING_PAGE_SETTING_PAGE_H_
#define PAGE_SETTING_PAGE_SETTING_PAGE_H_

#include "ui.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	int temp_threshold;
	int hum_threshold;
	int indecrement;
} Threshold;
typedef struct {
	bool led1;
	bool led2;
} ObLed;

void temp_hum_threshold_init(Threshold threshold_init);
int temp_inc(bool is_inc);
int hum_inc(bool is_inc);
ObLed compare(int temp, int ldr);
void save_threshold(void);
Threshold get_threshold(void);



#endif /* PAGE_SETTING_PAGE_SETTING_PAGE_H_ */

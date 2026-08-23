/*
 * setting_page.c
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */
#include "setting_page.h"

static Threshold threshold;
static Threshold unsave_threshold;

void temp_hum_threshold_init(Threshold threshold_init){
	threshold.temp_threshold = threshold_init.temp_threshold;
	threshold.hum_threshold = threshold_init.hum_threshold;
	threshold.indecrement = threshold_init.indecrement;
}

int temp_inc(bool is_inc){
	if(unsave_threshold.temp_threshold >= 100 && unsave_threshold.temp_threshold <= 0)
		return -1;

	unsave_threshold.temp_threshold = is_inc ? unsave_threshold.temp_threshold + unsave_threshold.indecrement : unsave_threshold.temp_threshold - unsave_threshold.indecrement;

	return unsave_threshold.temp_threshold;
}

int hum_inc(bool is_inc){
	if(unsave_threshold.hum_threshold >= 100 && unsave_threshold.hum_threshold <= 0)
		return -1;

	unsave_threshold.hum_threshold = is_inc ? unsave_threshold.hum_threshold + unsave_threshold.indecrement : unsave_threshold.hum_threshold - unsave_threshold.indecrement;

	return unsave_threshold.hum_threshold;
}

ObLed compare(int temp, int hum) {
	// should return control whether to light or not light ob led
	ObLed obled = {
		.pg13 = false,
		.pg14 = false
	};

	if(temp > threshold.temp_threshold) {
		obled.pg13 = true;
		// motor function call
	}
	if(hum > threshold.hum_threshold) {
		obled.pg14 = true;
		// motor function call
	}

	return obled;

}
void save_threshold(void) {
	threshold.temp_threshold = unsave_threshold.temp_threshold;
	threshold.hum_threshold = unsave_threshold.hum_threshold;
}

Threshold get_threshold(void) {
	unsave_threshold = threshold;
	return threshold;
}



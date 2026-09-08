/*
 * sensor_page.c
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#include "sensor_page.h"

static lv_timer_t *sensor_timer = NULL;
extern char buffer[50];
static int save_count = 0;

void sensor_page_update(void){
	// read dht22 and ldr
    Sensor sensor = temp_hum_ldr();

    sprintf(buffer, "%d", (int)sensor.temp);
    lv_label_set_text(ui_TemperatureS, buffer);
    sprintf(buffer, "%d", (int)sensor.hum);
    lv_label_set_text(ui_HumidityS, buffer);
    sprintf(buffer, "%d", (int)sensor.ldr);
    lv_label_set_text(ui_IntensityS, buffer);

    ObLed obled = compare((int)sensor.temp, (int)sensor.hum);
    GPIO_openObLED(obled.pg13, obled.pg14);

    // save to nand flash here
    save_count++;
    sprintf(buffer, "%d", save_count);
    lv_label_set_text(ui_SaveS, buffer);

    if(save_count >= 5) {
    	// save to nand

    	save_count = 0;
    }

}

static void sensor_page_timer_callback(lv_timer_t *timer){
	// callback after 10 sec
    sensor_page_update();
}


void sensor_page_start(void){
	// the page loaded when you navigate in
	sprintf(buffer, "%d", save_count);
	lv_label_set_text(ui_SaveS, buffer);
    sensor_page_update();

    if(sensor_timer == NULL){
        sensor_timer = lv_timer_create(
            sensor_page_timer_callback,
            10000,
            NULL
        );
    }
}

void sensor_page_stop(void){
	// when leave sensor page, unloaded
    if(sensor_timer != NULL){
        lv_timer_del(sensor_timer);
        sensor_timer = NULL;
        GPIO_openObLED(false, false);
    }
}

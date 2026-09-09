/*
 * sensor_page.c
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#include "sensor_page.h"

static lv_timer_t *sensor_timer = NULL;
static int save_count = 0;
static LedGpio* led1;
static LedGpio* led2;
extern char buffer[50];

void sensor_page_init(LedGpio *ledgpio1, LedGpio *ledgpio2){
	led1 = ledgpio1;
	led2 = ledgpio2;
}

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
    obled.led1 ? open_led(led1) : close_led(led1);
    obled.led2 ? open_led(led2) : close_led(led2);

    // save to nand flash here
    NAND_log_add_record((int16_t)sensor.temp, (uint8_t)sensor.hum, (uint16_t)sensor.ldr);

    save_count++;
    sprintf(buffer, "%d", save_count);
    lv_label_set_text(ui_SaveS, buffer);

    if(save_count >= 5) {
    	save_count = 0;
    	sprintf(buffer, "%d", save_count);
	    lv_label_set_text(ui_SaveS, buffer);
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
        close_led(led1);
        close_led(led2);
    }
}

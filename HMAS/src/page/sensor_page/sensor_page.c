/*
 * sensor_page.c
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#include "sensor_page.h"

static lv_timer_t *sensor_timer = NULL;


void sensor_page_update(void)
{
	// read dht22 and ldr
    Sensor sensor = temp_hum_ldr();

    char temp_str[20];
    char hum_str[20];
    char ldr_str[20];

    /* DHT22 */
    if(sensor.excmsgDht22 == NULL){
        sprintf(temp_str, "%.1f C", sensor.temp);
        sprintf(hum_str, "%.1f %%", sensor.hum);

        lv_label_set_text(ui_TemperatureS, temp_str);
        lv_label_set_text(ui_HumidityS, hum_str);
        // call nand function to store, but havent implement
    } else {
    	lv_obj_clear_flag(ui_ErrorPanelS, LV_OBJ_FLAG_HIDDEN);
    	lv_obj_set_text(ui_ErrorLabel1S, sensor.excmsgDht22);
    	lv_obj_set_text(ui_ErrorLabel2S, "");
    }

    if(sensor.excmsgLdr == NULL){
        sprintf(ldr_str, "%.2f", sensor.ldr);
        lv_label_set_text(ui_IntensityS, ldr_str);

        // call nand function to store, but havent implement
    } else {

    }

    if(sensor.excmsgDht22 != NULL || sensor.excmsgLdr != NULL){
    	lv_obj_clear_flag(ui_ErrorPanelS, LV_OBJ_FLAG_HIDDEN);
    	//lv_obj_set_text(ui_ErrorLabel1S, sensor.excmsgDht22  sensor.excmsgDht22);
    	lv_obj_set_text(ui_ErrorLabel2S, sensor.excmsgLdr);
    }else{
    	lv_obj_add_flag(ui_ErrorPanelS, LV_OBJ_FLAG_HIDDEN);
    }
}

static void sensor_page_timer_callback(lv_timer_t *timer){
	// callback after 10 sec
    sensor_page_update();
}


void sensor_page_start(void){
	// the page loaded when you navigate in
    sensor_page_update();

    if(sensor_timer == NULL){
        sensor_timer = lv_timer_create(
            sensor_page_timer_callback,
            10000,
            NULL
        );
    }
}

void sensorpage_stop(void){
	// when leave sensor page, unloaded
    if(sensor_timer != NULL){
        lv_timer_del(sensor_timer);
        sensor_timer = NULL;
    }
}

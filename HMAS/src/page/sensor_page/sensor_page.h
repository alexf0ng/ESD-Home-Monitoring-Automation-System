/*
 * sensor_page.h
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */

#ifndef PAGE_SENSOR_PAGE_SENSOR_PAGE_H_
#define PAGE_SENSOR_PAGE_SENSOR_PAGE_H_

#include "ui.h"
#include <stdio.h>
#include "sensor/sensor.h"
#include "page/setting_page/setting_page.h"
#include "gpio/gpio.h"
#include "nand/nand.h"

void sensor_page_update(void);
void sensor_page_start(void);
void sensor_page_stop(void);


#endif /* PAGE_SENSOR_PAGE_SENSOR_PAGE_H_ */

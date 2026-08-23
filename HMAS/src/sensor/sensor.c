/*
 * sensor.c
 *
 *  Created on: Aug 23, 2026
 *      Author: User
 */
#include "sensor.h"
extern char buffer[50];

Dht22 temp_hum(void) {
	Dht22 dht22 = {
		.temp = 0,
		.hum = 0,
	};

	DHT22_Read();

	dht22.hum = DHT22getHumidity();
	sprintf(buffer, "hum = %d \r\n", (int)dht22.hum);
	USART1_send_string(buffer);


	dht22.temp = DHT22getTemperature();
	sprintf(buffer, "temp = %d \r\n", (int)dht22.temp);
	USART1_send_string(buffer);

	return dht22;

}

Ldr light_dependent_resistor(void){
	Ldr ldr = {
		.ldr = 0,
	};

	// haiya i didnt bring ldr back
	ldr.ldr = 10;

	return ldr;
}

Sensor temp_hum_ldr(void){
    Sensor sensor = {
        .temp = 0,
        .hum = 0,
        .ldr = 0,
    };

    Dht22 dht22 = temp_hum();
    Ldr ldr = light_dependent_resistor();

    sensor.temp = dht22.temp;
    sensor.hum = dht22.hum;
    sensor.ldr = ldr.ldr;

    return sensor;
}


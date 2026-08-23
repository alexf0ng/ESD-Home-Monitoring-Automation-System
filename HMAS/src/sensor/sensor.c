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
		.temp = 0.0f,
		.hum = 0.0f,
		.excmsgDht22 = NULL
	};

	if(!DHT22_Read()){
		dht22.excmsgDht22 = "DHT22 read failed";
		return dht22;
	}

	dht22.hum = DHT22getHumidity();

	dht22.temp = DHT22getTemperature();

	return dht22;

}

Ldr light_dependent_resistor(void){
	Ldr ldr = {
		.ldr = 0.0f,
		.excmsgLdr = NULL
	};

	// haiya i didnt bring ldr back
	ldr.ldr = 10.15;

	return ldr;
}

Sensor temp_hum_ldr(void){
    Sensor sensor = {
        .temp = 0.0f,
        .hum = 0.0f,
        .ldr = 0.0f,
        .excmsgDht22 = NULL,
        .excmsgLdr = NULL
    };

    Dht22 dht22 = temp_hum();
    Ldr ldr = light_dependent_resistor();

    sensor.temp = dht22.temp;
    sensor.hum = dht22.hum;
    sensor.ldr = ldr.ldr;

    sensor.excmsgDht22 = dht22.excmsgDht22;
    sensor.excmsgLdr = ldr.excmsgLdr;

    return sensor;
}


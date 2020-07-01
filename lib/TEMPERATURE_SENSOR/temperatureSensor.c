/*
 * temperatureSensor.c
 *
 *  Created on: 04.06.2020
 *      Author: tom
 */

#include "temperatureSensor.h"

float actualTemperature =0;

void task_temperatureSensor(){
	configDS18B20(1,1);
	actualTemperature = ReadTempX2(1);
	ReadTempX1(1);
}

void init_temperatureSensor(){
	ds18b20_initialize();
}
float getActualTemperature(){
	return actualTemperature;
}

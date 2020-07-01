/*
 * temperature_sensor.h
 *
 *  Created on: 04.06.2020
 *      Author: tom
 */

#ifndef LIB_TEMPERATURE_SENSOR_TEMPERATURESENSOR_H_
#define LIB_TEMPERATURE_SENSOR_TEMPERATURESENSOR_H_

#include "ds18b20.h"

void task_temperatureSensor();
void init_temperatureSensor();
float getActualTemperature();

#endif /* LIB_TEMPERATURE_SENSOR_TEMPERATURESENSOR_H_ */

/*
 * measureVoltege.h
 *
 *  Created on: 26.05.2020
 *      Author: tom
 */

#ifndef LIB_ADC_VOLTAGEMEASUREMENT_H_
#define LIB_ADC_VOLTAGEMEASUREMENT_H_

#include <lib/ADC/adc.h>  //temp
#include <lib/GSM/m95.h>

void initVoltageMeasurement_g();
void taskVoltageMeasurement_g();
float getActualVoltageMeasurment_g();
uint8_t isVoltageToLow();

#endif /* LIB_ADC_VOLTAGEMEASUREMENT_H_ */

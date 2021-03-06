/*
 * measureVoltege.c
 *
 *  Created on: 26.05.2020
 *      Author: tom
 */
#include <lib/ADC/voltageMeasurement.h>

#define MINIMAL_VOLTAGE_FLOAT ((float)(11.8))
#define MINIMAL_VOLTAGE_HYSTERESIS_FLOAT ((float)(0.3))

float actualVoltageMeasurment;
uint8_t errorVoltageToLow = 0;

void voltageMeasurement_l();
void sendSmsError();

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void initVoltageMeasurement_g() {
	adc_init();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void taskVoltageMeasurement_g() {
	voltageMeasurement_l();
	if (MINIMAL_VOLTAGE_FLOAT > actualVoltageMeasurment) {
		if(0==errorVoltageToLow)
		{
			sendSmsError();
		}
		errorVoltageToLow = 1;
	}
	if ((MINIMAL_VOLTAGE_FLOAT+MINIMAL_VOLTAGE_HYSTERESIS_FLOAT) < actualVoltageMeasurment) {
		errorVoltageToLow = 0;
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
float getActualVoltageMeasurment_g() {
	return actualVoltageMeasurment;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void voltageMeasurement_l() {
	actualVoltageMeasurment = meausreVoltage();

}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
uint8_t isVoltageToLow() {
	return errorVoltageToLow;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sendSmsError(){//TODO
	m95SendSMS("515243059", "ALARM AKUMULATOR - voltage too low");
}

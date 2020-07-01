
#ifndef DS18B20_H_
#define DS18B20_H_


#define portW1 GPIOA
//#define wire1_s0 GPIO_Pin_2  //12
#define wire1_s1 GPIO_Pin_3  //15

#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

void ds18b20_initialize();
//float ReadTemp(int sensor);

void ReadTempX1(int sensor);
float ReadTempX2(int sensor);
void configDS18B20(uint8_t config, int sensor);

uint16_t wire1;

#endif /* DS18B20_H_ */

/*
 * m95-stm32.h
 *
 *  Created on: 27 gru 2014
 *      Author: tom
 */

#ifndef LIB_GSM2_M95_STM32_H_
#define LIB_GSM2_M95_STM32_H_
//#include "usart.h"
#include "stm32f0xx.h"

#define M95_USART USART1

void m95UsartSend_____t(char * stringToWrite);
void m95UsartSendCtrlZ();
void m95UsartWait();



int index_buffor;
char buffor[250];

#endif /* LIB_GSM2_M95_STM32_H_ */

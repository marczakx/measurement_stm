/*
 * usart.h
 *
 *  Created on: 23.05.2020
 *      Author: tom
 */

#ifndef LIB_USART_USART_H_
#define LIB_USART_USART_H_
#include "stm32f0xx.h"

void initUsart();
void sendUsart(char * stringToWrite);
void Usart_WriteString(USART_TypeDef *USARTx, char * stringToWrite);

#endif /* LIB_USART_USART_H_ */

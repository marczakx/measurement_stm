

#include <lib/GSM/m95-stm32.h>
#include <lib/USART/usart.h>
void m95UsartSend_____t(char * stringToWrite) {
	Usart_WriteString(M95_USART, stringToWrite);
	//sendUsart(stringToWrite); ///log
}
void m95UsartSendCtrlZ() {
	Usart_ctrlz(M95_USART);
}
void m95UsartWait() {
	volatile int c;
	for (int y = 0; y < 100; y++)

		for (int i = 0; i < 10000; i++) {
			asm("nop");
			c = i + y;
		}
}

void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		int buffor_l = 250;
		char ascii_znak_usart1 = USART_ReceiveData(USART1);
		USART_SendData(USART2, ascii_znak_usart1); //////////////////////-----------log
		buffor[index_buffor] = ascii_znak_usart1;
		index_buffor++;
		if (index_buffor >= buffor_l)
			index_buffor = 0;
	}

}

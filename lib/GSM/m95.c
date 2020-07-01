#include <lib/GSM/m95.h>

#define SIZE_DATA_U8   ((uint8_t)(30))

typedef enum {
	GSM_GPRS_STATE_INIT_E,
	GSM_GPRS_STATE_DISCONECTED_E,
	GSM_GPRS_STATE_CONNECTED_E
} GSM_GPRS_STATE_EN_TYPE;

char flagIsSomethingToSend;
char dataToSend[SIZE_DATA_U8];
GSM_GPRS_STATE_EN_TYPE gsmGprsState_EN = GSM_GPRS_STATE_INIT_E;
char recivedStatus = 0;
char * host = "rocky-gorge-50247.herokuapp.com";
char ip[20];
char ipStatus = 0;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void initM95PinOn() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GSM_PWRKEY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GSM_PWRKEY_PORT, &GPIO_InitStructure);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void initM95Usart() {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); // USART1 TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  //Usart1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;  // 6500;//9600;//6700;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//////////////////////
	USART_Init(USART1, &USART_InitStructure);
	//	 USART_Init(USART2, &USART_InitStructure);
	//USART_Cmd(USART2,ENABLE);
	USART_Cmd(USART1, ENABLE);

	//
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void initM95() {
	initM95PinOn();
	initM95Usart();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void m95On(void) {
	GSM_PWRKEY_PORT->BRR = GSM_PWRKEY_PIN;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void m95Off(void) {
	GSM_PWRKEY_PORT->BSRR = GSM_PWRKEY_PIN;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sendToM95(char *s) {
	m95UsartSend_____t("\r");
	m95UsartSend_____t(s);
	m95UsartSend_____t("\r");

}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void m95SendSMS(char * number, char * message) {

	sendToM95("at+cmgf=1");

	m95UsartWait();

	sendToM95("at+cscs=\"gsm\"");

	m95UsartWait();
	m95UsartSend_____t("at+cmgs=\"");
	m95UsartSend_____t(number);
	m95UsartSend_____t("\"\r");
	m95UsartWait();

	m95UsartSend_____t(message);
	m95UsartWait();
	m95UsartWait();

	m95UsartSendCtrlZ();
	m95UsartWait();

}

/////////////////////////////////////////////////////////////////////
char command[250];
/////////////////////////////////////////////////////////////////////
char gsm_resive_command;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void resiveCharFromM95Usart() {
	static int index_buffor_ti = 0;
	static int command_index = 0;
	if (index_buffor_ti != index_buffor) {
		if (buffor[index_buffor_ti] == '\r') {
			if (command_index != 0) {
				command[command_index] = 0;
				command_index = 0;
				gsm_resive_command = 1;
			}
		} else {
			if (buffor[index_buffor_ti] != 0)
				if (buffor[index_buffor_ti] != '\n') {
					command[command_index] = buffor[index_buffor_ti];
					command_index++;
				}
		}
		index_buffor_ti++;
		int buffor_l = 250;
		if (index_buffor_ti >= buffor_l)
			index_buffor_ti = 0;
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

char compare(char* firstText, char*secondText) {
	while (*firstText) {
		if (*firstText != *secondText)
			return 0;
		firstText++;
		secondText++;
	}
	if (*firstText != *secondText)
		return 0;
	return 1;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void checkIsStatus(char* line) {

	if (compare(line, "AT")) {
		return;
	}

	if (compare(line, "ERROR")) {
		recivedStatus = 0;
		return;
	}

	if (GSM_GPRS_STATE_INIT_E == gsmGprsState_EN) {
		if (compare(line, "OK")) {
			recivedStatus = 1;
		}
		return;
	}
	if (GSM_GPRS_STATE_DISCONECTED_E == gsmGprsState_EN) {
		if (compare(line, "CONNECT OK")) {
			recivedStatus = 1;
		}
		return;
	}
	if (GSM_GPRS_STATE_CONNECTED_E == gsmGprsState_EN) {
		if (compare(line, "SEND OK")) {
			recivedStatus = 1;
		}
		return;
	}

}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
char* getIp() {
	ipStatus = 0;
	return ip;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
char isIpFounded() {
	return ipStatus;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void checkIsIP(char* line) {
	if ((line[2] == '.' && line[6] == '.')
			|| (line[3] == '.' && line[7] == '.')) {//todo regular expression
		ipStatus = 1;
		char index = 0;
		while (line[index]) {
			ip[index] = line[index];
			index++;
		}
		ip[index] = line[index];
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void task1_GSM() {
	resiveCharFromM95Usart();

	if (gsm_resive_command) {
		gsm_resive_command = 0;

		checkIsStatus(command);
		checkIsIP(command);
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
char getReciveState() {
	if (recivedStatus) {
		recivedStatus = 0;
		return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void gsmGprsInitState() {
	m95On();
	char reciveState = getReciveState();
	if (reciveState) {
		gsmGprsState_EN = GSM_GPRS_STATE_DISCONECTED_E;
		return;
	}
	sendToM95("AT");
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
char isSomethingToSend() {
	return flagIsSomethingToSend;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void gsmGprsDisconectedState() {
	if (isSomethingToSend()) {
		m95On();////////////////////////////////- ---move to state init or off

		char reciveState = getReciveState();
		if (reciveState) {
			gsmGprsState_EN = GSM_GPRS_STATE_CONNECTED_E;
			return;
		}
		if (isIpFounded()) {
			connectToHer(getIp());
			return;
		}
		m95UsartSend_____t("AT+QIDNSGIP=\"");
		m95UsartSend_____t(host);
		m95UsartSend_____t("\"\n\r");

		return;
	}

}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void gsmGprsConnectedState() {

	char reciveState = getReciveState();
	if (reciveState) {
		flagIsSomethingToSend = 0;
		gsmGprsState_EN = GSM_GPRS_STATE_DISCONECTED_E;

#ifdef TURNOF_TO_SAVE_ENERGY
		sendToM95("AT+QICLOSE");
		m95Off();
		sendToM95("AT+QPOWD=1");
#endif
		return;

	}
	sendToHer(dataToSend, host);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void task2_GSM() {

	if (GSM_GPRS_STATE_INIT_E == gsmGprsState_EN) {
		gsmGprsInitState();
		return;
	}
	if (GSM_GPRS_STATE_DISCONECTED_E == gsmGprsState_EN) {
		gsmGprsDisconectedState();
		return;
	}
	if (GSM_GPRS_STATE_CONNECTED_E == gsmGprsState_EN) {
		gsmGprsConnectedState();
		return;
	}
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void connectToHer(char* ip) {
	m95UsartSend_____t("AT+QIOPEN=\"TCP\",\"");
	m95UsartSend_____t(ip);
	m95UsartSend_____t("\",\"80\"\r\n");
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sendToHer(char* somethingToSend, char*host) {
	m95UsartSend_____t("AT+QISEND\r\n");
	m95UsartWait();
	m95UsartSend_____t("GET /");
	m95UsartSend_____t(somethingToSend);
	m95UsartSend_____t(" HTTP/1.1\r\n");
	m95UsartSend_____t("Host:");
	m95UsartSend_____t(host);
	m95UsartSend_____t("\r\n");
	m95UsartSend_____t("Connection:keep-alive\r\n");
	m95UsartSend_____t("\r\n");
	m95UsartSendCtrlZ();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void Usart_ctrlz(USART_TypeDef *USARTx) {

	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USARTx, 26);
	for (int i = 0; i > 10000; i++)
		asm("nop");

}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void setMessageToSend(char*textFirstPart, char*textSecondPart) {
	char index_U8 = 0;
	while (*textFirstPart && (SIZE_DATA_U8 - 1) > index_U8) {
		dataToSend[index_U8] = *textFirstPart;
		textFirstPart++;
		index_U8++;
	}
	while (*textSecondPart && (SIZE_DATA_U8 - 1) > index_U8) {
		dataToSend[index_U8] = *textSecondPart;
		textSecondPart++;
		index_U8++;
	}
	dataToSend[index_U8] = 0;
	flagIsSomethingToSend = 1;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sendTemperature_gprs(char*message) {
	setMessageToSend("data/saveTemperature", message);
	flagIsSomethingToSend = 1;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sendVoltage_gprs(char*message) {

	setMessageToSend("data/save", message);
	flagIsSomethingToSend = 1;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sendSensor5_gprs(char*message) {
	setMessageToSend("data/saveid5/", message);	//TODO
	flagIsSomethingToSend = 1;
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void sendSensor6_gprs(char* message) {
	setMessageToSend("data/saveid6/", message);	//TODO
	flagIsSomethingToSend = 1;
}


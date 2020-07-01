

//source https://stm32.eu/2012/05/10/zl31arm-obsluga-ds18b20/


#include <lib/TEMPERATURE_SENSOR/ds18b20.h>

void initTimerForDS18B20() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //w����czenie zegara

	TIM_TimeBaseInitTypeDef TIM_InitStruct; //struktura inicjalizacyjna

	TIM_InitStruct.TIM_ClockDivision = 0; //dzielnik 0
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; // licznik w gore
	TIM_InitStruct.TIM_Period = 65535; // okres licznika
	TIM_InitStruct.TIM_Prescaler = 72; // preskaler 72
	TIM_Cmd(TIM2, ENABLE);
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct); // inicjalizacja TIM2
}

void ds18b20_initialize() {
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = wire1_s1;
	GPIO_Init(portW1, &GPIO_InitStructure);

	initTimerForDS18B20();

}
void ds_delay(uint16_t time1) {
	TIM_SetCounter(TIM2, 0); //zerowanie licznika
	while (TIM_GetCounter(TIM2) <= time1)
		; //odczyt CNT i porównanie
}

uint16_t RESET_PULSE(void) {
	uint16_t PRESENCE = 0;
	GPIO_ResetBits(portW1, wire1); //ustawienie stanu niskiego
	ds_delay(480); //odczekanie 480us
	GPIO_SetBits(portW1, wire1); //odpuszczenie do stanu wysokiego
	ds_delay(70); //odczekanie ok. 70us na reakcje czujnika

	//sprawdzenie odpowiedzi
	if (GPIO_ReadInputDataBit(portW1, wire1) == Bit_RESET)
		PRESENCE++;
	ds_delay(410);

	if (GPIO_ReadInputDataBit(portW1, wire1) == Bit_SET)
		PRESENCE++;

	if (PRESENCE == 2)
		return 1;
	else
		return 0;
}

void SendBit(uint16_t bit) {
	if (bit == 0) {
		GPIO_ResetBits(portW1, wire1);
		ds_delay(65);
		GPIO_SetBits(portW1, wire1);
		ds_delay(10);
	} else {
		GPIO_ResetBits(portW1, wire1);
		ds_delay(10);
		GPIO_SetBits(portW1, wire1);
		ds_delay(65);
	}
}

uint16_t ReadBit(void) {
	uint16_t bit = 0;
	GPIO_ResetBits(portW1, wire1);
	ds_delay(5);
	GPIO_SetBits(portW1, wire1);
	ds_delay(5);

	//warunek sprawdzający czy zostało wysłane 1 czy 0
	if (GPIO_ReadInputDataBit(portW1, wire1) == Bit_SET)
		bit = 1;
	else
		bit = 0;

	ds_delay(55);  	//przerwa czasowa między bitami
	return bit;
}

void SendByte(uint16_t value) {
	uint16_t i, tmp;
	for (i = 0; i < 8; i++) {
		tmp = value >> i; //przesunięcie bitowe
		tmp &= 0x01; //wyodrębnienie bitu do wysłania
		SendBit(tmp); //wysłanie bitu
	}
}

uint16_t ReadByte(void) {
	uint16_t i, value = 0;
	for (i = 0; i < 8; i++) {
		if (ReadBit()) //odczyt linii danych
			value |= 0x01 << i; //tworzenie ośmiobitowej liczby
	}
	return value;
}

void ReadTempX1(int sensor) {
	//wire1=0;
	uint16_t i, presence = 0;

	if (sensor == 0) {
		//	wire1=wire1_s0;
	}
	if (sensor == 1) {
		wire1 = wire1_s1;
	}

	presence = RESET_PULSE();

	if (presence == 1) {
		SendByte(0xCC); //Skip ROM
		SendByte(0x44); //Convert T
	} else
		return 88.8;
}
float ReadTempX2(int sensor) {
	float temp = 0;
	uint16_t presence = 0, memory[3];
	if (sensor == 0) {
		//	wire1=wire1_s0;
	}
	if (sensor == 1) {
		wire1 = wire1_s1;
	}


	presence = RESET_PULSE();

	if (presence == 1) {
		SendByte(0xCC);	//Skip ROM
		SendByte(0xBE);	//Read Scratchpad

		for (int i = 0; i < 2; i++)
			memory[i] = ReadByte(); 		//odczyt 2 bajtów Scratchpad

		memory[2] = (240U & memory[1]) >> 7;
		memory[1] = (15U & memory[1]) << 8;

		if (memory[2] == 0)	//jeśli dodatnia temperatura
			temp = (memory[0] + memory[1]) / ((float) 16.0);
		if (memory[2] == 1)		//jeśli ujemna temperatura
			temp = (memory[0] + memory[1] - 4095) / ((float) 16.0);
		//	temp = (memory[0] + memory[1] -4095)/16.0;
	} else
		return 99.9;

	presence = RESET_PULSE();
	return temp;
}
void copyScratchpadDS18B20(int sensor) {

	uint16_t presence = 0;
	if (sensor == 0) {
		//	wire1=wire1_s0;
	}
	if (sensor == 1) {
		wire1 = wire1_s1;
	}

	presence = RESET_PULSE();

	if (presence == 1) {
		SendByte(0xCC);	//Skip ROM
		SendByte(0x48);

	}

	presence = RESET_PULSE();

}
void configDS18B20(uint8_t config, int sensor) {
	float temp = 0;
	uint16_t presence = 0, memory[3];
	if (sensor == 0) {
//		wire1=wire1_s0;
	}
	if (sensor == 1) {
		wire1 = wire1_s1;
	}

	presence = RESET_PULSE();

	if (presence == 1) {
		SendByte(0xCC);	//Skip ROM
		SendByte(0x4E);

		SendByte(0x7F);
		SendByte(0x00);	//tl
		SendByte(0x7F);	//config

	}

	presence = RESET_PULSE();
	copyScratchpadDS18B20(sensor);
}

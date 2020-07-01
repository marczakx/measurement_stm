
#ifndef LIB_GSM2_M95_H_
#define LIB_GSM2_M95_H_

#define GSM_PWRKEY

#define GSM_PWRKEY_PORT						GPIOB
#define GSM_PWRKEY_PIN						GPIO_Pin_0

#include <lib/GSM/m95-stm32.h>


char * telephon_number;
char last_telephon_number[15];

void initM95();
void m95On(void);
void m95Off(void);

void m95SignalStrength();
void gsm_command(char * command);
int getZasieg();
void decRing();
void getRingNumber(char* command);
void sendToM95(char *s);
void readSMS(char* c);
void commandToSystem(char* c);
void sendSmsMeasure();
uint8_t checkTelefonNumber();
void deleteAllMessages();

void sendTemperature_gprs(char*message);
void sendVoltage_gprs(char*message);
void sendSensor5_gprs(char*message);
void sendSensor6_gprs(char* message);

void task1_GSM();
void task2_GSM();

#endif /* LIB_GSM2_M95_H_ */

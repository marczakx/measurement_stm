#include <lib/ADC/adc.h>

float w_a = 0;
float w_b = 0;

void adc_init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;     // PA.1 ADC IN1444
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitTypeDef ADC_InitStructure;

	ADC_DeInit(ADC1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* Convert the ADC1 input with 55.5 Cycles as sampling time */
	ADC_ChannelConfig(ADC1, ADC_Channel_4, ADC_SampleTime_55_5Cycles); // PA.1 - IN1  4444

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait the ADCEN falg */
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN))
		;

	/* ADC1 regular Software Start Conv */
	ADC_StartOfConversion(ADC1);
}


uint16_t getADC1() {
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		;
	return ADC_GetConversionValue(ADC1);
}


float getADC(int l) {
	float result_f;
	uint32_t sum_U32 = 0;
	int i = 0;
	for (i = 0; i < l; i++) {
		uint16_t adcResult_U16 = getADC1();
		uint32_t diffMaxAndResult_U32 = ((uint32_t) (0xFFFFFFFF
				- (uint32_t) adcResult_U16));
		if (diffMaxAndResult_U32 > sum_U32) {
			sum_U32 += (uint32_t) adcResult_U16;
		} else {
			break;
		}
	}
	result_f = sum_U32 / (float)i;
	return result_f;
}


void set_w(float a, float b) {
	w_a = a;
	w_b = b;
}

float meausreVoltage() {

	float adc = getADC(10000);
	// old set_w(0.007534, -1.747);
	set_w(0.007084, 0.03340);
	return w_a * adc + w_b;
}


#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

void ADC_init(){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//	ADC1->CR2 |= ADC_CR2_CAL;
//	while (!(ADC1->CR2 & ADC_CR2_CAL));
//	ADC1 -> CR1 = ADC_CR1_EOCIE;
	ADC1 -> CR2 |= ADC_CR2_EXTSEL | ADC_CR2_EXTEN;
	ADC1 -> SMPR3 |= ADC_SMPR3_SMP0_1;
	ADC1 -> CR2 |= ADC_CR2_ADON;
}

unsigned int ADC_read(unsigned int chanel){
	ADC1 -> SQR3 |=  chanel;
	ADC1 -> CR2 |= ADC_CR2_SWSTART;
	while (!(ADC1 -> SR & ADC_SR_EOC)){

	}
	return ADC1 -> DR;
}


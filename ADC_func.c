//#include "ADC_func.h"
//
//void ADC_init(){
//	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//	ADC1 -> SMPR2 |= ADC_SMPR2_SMP18_1 | ADC_SMPR2_SMP18_2 ;
//	ADC1 -> CR2 |= ADC_CR2_ADON;
//	while(!(ADC1->SR&ADC_SR_ADONS));
//	ADC1 -> CR1 |= ADC_CR1_EOCIE;
////	ADC1->CR2 |= ADC_CR2_CAL;
////	while (!(ADC1->CR2 & ADC_CR2_CONT));
////	ADC1 -> CR1 = ADC_CR1_EOCIE;
////	ADC1 -> CR2 |= ADC_CR2_EXTSEL | ADC_CR2_EXTEN;
//
//
//}
//
//unsigned int ADC_read(uint32_t chanel){
//	 ADC1->SQR5 = chanel;
//	ADC1 -> CR2 |= ADC_CR2_SWSTART;
//	while (!(ADC1 -> SR & ADC_SR_EOC)){
//
//	}
//	return ADC1 -> DR;
//}
//

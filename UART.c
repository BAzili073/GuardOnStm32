#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"

#define TCHK ;

char gsm_buffer[GSM_BUFFER_SIZE];
uint16_t gsm_buffer_char_counter = 0;
void send_char_to_GSM(char c);
void USART_get_message();

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;


void UART1_init(){
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
	NVIC_EnableIRQ(USART1_IRQn);
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);
	USART1->CR1 |= USART_CR1_RXNEIE;
}


void send_string_to_GSM(char * s){
	int i = 0;
	while(s[i] != '\0')
	{
		send_char_to_GSM(s[i]);
		i++;
	}
}

void send_char_to_GSM(char c){
	while((USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = c;
}

void USART1_IRQHandler(){
		 USART_get_message();
		 USART1->SR &= ~(USART_IT_RXNE | USART_SR_ORE);
}

void USART_get_message(){
	gsm_buffer[gsm_buffer_char_counter] = USART1 -> DR;
	gsm_buffer_char_counter++;
	if (gsm_buffer_char_counter == GSM_BUFFER_SIZE) gsm_buffer_char_counter = 0;
}



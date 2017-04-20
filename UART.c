#include "defines.h"
#include INCLUDED_FILE_1
#include INCLUDED_FILE_2

char gsm_buffer[GSM_BUFFER_SIZE];
char uart2_buffer[UART2_BUFFER_SIZE];

unsigned int gsm_buffer_char_counter = 0;
unsigned int uart2_buffer_char_counter = 0;


void send_char_to_GSM(char c);
void USART_get_message();
void USART2_get_message();

volatile unsigned int uart2_check_counter = 0;
char UART2_message[UART2_MESSAGE_SIZE];
uint16_t uart_digit(uint16_t dig, uint16_t sub);

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;


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

void UART2_init(){
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
	NVIC_EnableIRQ(USART2_IRQn);
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;//9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart2);
	USART2->CR1 |= USART_CR1_RXNEIE ;
}

void UART3_init(){
	RCC -> APB1ENR |= RCC_APB1ENR_LPUART1EN;
//	NVIC_EnableIRQ(USART3_IRQn);
	huart3.Instance = LPUART1;
	huart3.Init.BaudRate = 9600;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart3);
	LPUART1->CR1 |= USART_CR1_RXNEIE;
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
	while((USART1->ISR & USART_ISR_TXE) == 0);
	USART1->TDR = c;
}
void send_char_to_UART2(char c){
	USART2->TDR = c;
	while((USART2->ISR & USART_ISR_TXE) == 0);
}

void send_string_to_UART2(char * s){
	int i = 0;
	while(s[i] != '\0')
	{
		send_char_to_UART2(s[i]);
		i++;
	}
}

void send_char_to_UART3(char c){
	while((LPUART1->ISR & USART_ISR_TXE) == 0);
	LPUART1->TDR = c;
}

void send_string_to_UART3(char * s){
	int i = 0;
	while(s[i] != '\0')
	{
		send_char_to_UART3(s[i]);
		i++;
	}
}



void USART1_IRQHandler(){
		 USART_get_message();
		 USART1->ISR &= ~(USART_IT_RXNE | USART_ISR_ORE);
}

void USART2_IRQHandler(){
		 USART2_get_message();
		 USART2->ISR &= ~(USART_IT_RXNE | USART_ISR_ORE);
}
void USART3_IRQHandler(){
		 LPUART1->ISR &= ~(USART_IT_RXNE | USART_ISR_ORE);
}

void USART_get_message(){
	gsm_buffer[gsm_buffer_char_counter] = USART1 -> TDR;
	gsm_buffer_char_counter++;
	if (gsm_buffer_char_counter == GSM_BUFFER_SIZE) {
		gsm_buffer_char_counter = 0;
	}
}

void USART2_get_message(){
	uart2_buffer[uart2_buffer_char_counter] = USART2 -> TDR;
	uart2_buffer_char_counter++;
	if (uart2_buffer_char_counter == UART2_BUFFER_SIZE) {
		uart2_buffer_char_counter = 0;
	}
}

void send_int_to_GSM(uint16_t num){
	char d1,d2,d3,d4;
	d1 = (num/1000);
	if (num>999) send_char_to_GSM((d1+'0'));
	d2 = ((num - d1 * 1000)/100);
	if (num>99) send_char_to_GSM((d2 + '0'));
	d3 = ((num - d1*1000 - d2*100)/10);
	if (num>9) send_char_to_GSM((d3 + '0'));
	d4 = ((num - d1*1000 - d2*100 - d3*10));
	send_char_to_GSM((d4 + '0'));
}

void send_num_to_UART3(uint32_t num){
	if(num > 0){
		send_num_to_UART3(num / 10);
		send_char_to_UART3((num % 10) + '0');
	}
}

void send_int_to_UART3(uint32_t num){
if(num > 0){
	send_num_to_UART3(num);
}else if (num == 0){
	send_char_to_UART3('0');
}else if (num < 0){
	send_char_to_UART3('-');
	num = -num;
	send_num_to_UART3(num);
}
//	char d1,d2,d3,d4;
//	if (num < 0) {
//		send_char_to_UART3('-');
//		num = num*(-1);
//	}
//	d1 = (num/1000);
//	if (num>999) send_char_to_UART3((d1+'0'));
//	d2 = ((num - d1 * 1000)/100);
//	if (num>99) send_char_to_UART3((d2 + '0'));
//	d3 = ((num - d1*1000 - d2*100)/10);
//	if (num>9) send_char_to_UART3((d3 + '0'));
//	d4 = ((num - d1*1000 - d2*100 - d3*10));
//	send_char_to_UART3((d4 + '0'));
}


char UART2_get_next_data(){
	if (uart2_buffer_char_counter == uart2_check_counter) return 0;
	int i;
	while (!((uart2_buffer[uart2_check_counter] == 0xAA) & (uart2_buffer[uart2_check_counter+1] == 0x55))){
		uart2_check_counter++;
		if (uart2_check_counter == UART2_BUFFER_SIZE) uart2_check_counter = 0;
		if (uart2_buffer_char_counter == uart2_check_counter) return 0;
}
		for (i = 0;i<26;i++){
			if (uart2_buffer_char_counter == uart2_check_counter) return 1;
			UART2_message[i] = uart2_buffer[uart2_check_counter];
			uart2_check_counter++;
			if (uart2_check_counter == UART2_BUFFER_SIZE) uart2_check_counter = 0;
			//if (gsm_buffer_char_counter == (uart2_check_counter)) return 0;
		}
		return 1;

}


void UART2_clear_buffer(){
	uart2_buffer_char_counter = uart2_check_counter;
}

char UART2_check_buffer(){
	if (uart2_buffer_char_counter == uart2_check_counter) return  0;
	return 1;
}

void UART2_clear_message(){
	int i = 0;
	for (i = 0;i<UART2_MESSAGE_SIZE;i++){
		UART2_message[i] = 0;
	}
}




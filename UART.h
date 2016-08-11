#ifdef UART_H
#else
#define UART_H
uint8_t gsm_buffer_char_counter;
char gsm_buffer[GSM_BUFFER_SIZE];

void UART1_init();
void send_string_to_GSM(char * s);
void send_char_to_GSM(char c);
void USART1_IRQHandler();
#endif

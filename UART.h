#ifdef UART_H
#else
#define UART_H
unsigned int gsm_buffer_char_counter;
char gsm_buffer[GSM_BUFFER_SIZE];

void UART1_init();
void send_string_to_GSM(char * s);
void send_char_to_GSM(char c);
void USART1_IRQHandler();
void send_int_to_GSM(uint16_t num);
#endif

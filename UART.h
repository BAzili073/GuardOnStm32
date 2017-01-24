#ifdef UART_H
#else
#define UART_H

#include "defines.h"


#define UART1_PIN_RX GPIO_PIN_9
#define UART1_PIN_TX GPIO_PIN_10
#define UART1_PORT GPIOA

#define UART2_PIN_RX GPIO_PIN_2
#define UART2_PIN_TX GPIO_PIN_3
#define UART2_PORT GPIOA


#define UART3_PIN_RX GPIO_PIN_10
#define UART3_PIN_TX GPIO_PIN_11
#define UART3_PORT GPIOB



unsigned int gsm_buffer_char_counter;
char gsm_buffer[GSM_BUFFER_SIZE];

char UART2_message[UART2_MESSAGE_SIZE];

void UART1_init();
void UART2_init();
void UART3_init();

void send_string_to_GSM(char * s);
void send_char_to_GSM(char c);
void send_int_to_GSM(uint16_t num);


void send_string_to_UART2(char * s);
void send_char_to_UART2(char c);
void UART2_clear_buffer();
char UART2_check_buffer();
void UART2_clear_message();
char UART2_get_next_data();


void send_string_to_UART3(char * s);
void send_char_to_UART3(char c);
void send_int_to_UART3(uint32_t num);




#endif

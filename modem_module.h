#ifdef MODEM_MODULE_H
#else
#define MODEM_MODULE_H


#include "defines.h"

char output_sms_message[70];
char input_sms_message[70];
char gsm_message[GSM_MESSAGE_SIZE];
int16_t parse_int_in_message(char * mes,uint8_t start);
void read_numbers();
void modem_time();
void clear_output_sms_message();
int get_gsm_signal_quality();
typedef struct TEL_obj{
	char number[11];
	uint8_t  access;
} TEL_obj;

#endif

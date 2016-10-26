#include "defines.h"
void MODEM_ON();
char modem_call(char * number);
char modem_send_sms_message(char * number,char * text);
char send_sms_message_for_all(char * text,int function);
char modem_setup();
char check_number_of_sms();
void convert_number_to_upd(char * number);
void ucs_to_eng(char * in_message, char * message);
void send_text_as_ucs(char * out_message, unsigned int len);
char check_gsm_message();
int modem_state;
char modem_time_check;
char input_sms_message[70];
char output_sms_message[70];
char gsm_message[GSM_MESSAGE_SIZE];
void modem_check_state();

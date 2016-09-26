int check_gsm_messages();
void MODEM_ON();
char modem_call(char * number);
char modem_send_sms_message(char * number,char * text);
char modem_setup();
char check_number_of_sms();

int modem_state;
char sms_message[70];
char gsm_message[GSM_MESSAGE_SIZE];

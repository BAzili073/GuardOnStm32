int check_gsm_messages();
void MODEM_ON();
int modem_state;
char modem_call(char * number);
char modem_send_sms_message(char * number,char * text);
char modem_setup();

char gsm_message[GSM_MESSAGE_SIZE];

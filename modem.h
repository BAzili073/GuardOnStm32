#ifdef MODEM_H
#else
#define MODEM_H

#include "defines.h"

#define MODEM_ON_PORT GPIOB
#define MODEM_ON_PIN GPIO_PIN_2
#define OK_ANSWER 1
#define NO_ANSWER 0

#define TEL_NUMBER_ALLOW 1
#define TEL_NUMBER_DENY 100

#define MODEM_STATE_OFF 0
#define MODEM_STATE_ON 1
#define MODEM_STATE_NO_SIM 2
#define MODEM_STATE_OFFLINE 3
#define MODEM_STATE_ONLINE 4
#define MODEM_STATE_SETUP 5
#define MODEM_STATE_STARTS 6
#define MODEM_STATE_NOT_NEED 7

#define MODEM_ACTION_FREE 0
#define MODEM_ACTION_OUTGOING_CALL 1
#define MODEM_ACTION_SMS 3
#define MODEM_ACTION_GPRS 4
#define MODEM_ACTION_INCOMING_CALL 5
#define MODEM_ACTION_TALK 6

#define GSM_MESSAGE_UNKNOW 0
#define GSM_MESSAGE_OK 1
#define GSM_MESSAGE_ERROR 2
#define GSM_MESSAGE_INCOMING_SMS 3
#define GSM_MESSAGE_NO_SIM 4
#define GSM_MESSAGE_COPS 5
#define GSM_MESSAGE_CALL_BUSY 6
#define GSM_MESSAGE_CALL_NO_ANSWER 7

char modem_errors[2];
#define MODEM_ERRORS_SEND_SMS 0
#define MODEM_ERRORS_NO_CARRIER 1

////FUNCTION
void MODEM_ON();
void MODEM_OFF();
void modem_call(char * number);
char modem_send_sms_message(char * number,char * text);
char send_sms_message_for_all(char * text,int function);
char modem_setup();
char check_number(char * number);
void convert_number_to_upd(char * number);
void ucs_to_eng(char * in_message, char * message);
//void send_text_as_ucs(char * out_message, unsigned int len);
char check_gsm_message();
void modem_check_state();
void modem_save_number(char ID_number,char * number,uint8_t acc);
void incoming_call();
void incoming_sms();
void modem_no_carrier();
void convert_number_to_eng(char * number);
void modem_free();
//char send_command_to_GSM(char * s,char * await_ans, char * answer, int t_msec, int max_t);
//int modem_state = MODEM_STATE_OFF;


////VARIABLES
int modem_action;

#define CALL_STATE_NO_CALL 0
#define CALL_STATE_CALL 1
#define CALL_STATE_BUSY 2
#define CALL_STATE_NO_CARRIER 3
#define CALL_STATE_NO_ANSWER 4


//

#endif

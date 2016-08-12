#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "EEPROMfunc.h"
#include "defines.h"
#include "TIMs.h"
#include "UART.h"
#include "string.h"


char parse_gsm_message();
void clear_gsm_message();
char check_gsm_message();
void del_all_gsm_message();

int modem_state = MODEM_STATE_OFF;
char get_next_gsm_message();
uint16_t gsm_message_check_counter = 0;
char send_command_to_GSM(char * s,char * await_ans, char * answer, int t, int max_t);


char gsm_message[GSM_MESSAGE_SIZE];


void GSM_ON(){
	check_gsm_message();
	while(modem_state == MODEM_STATE_OFF){
		GPIO_LOW(MODEM_ON_PORT,MODEM_ON_PIN);
		set_timeout(800);
		while_timeout();
		GPIO_HIGH(MODEM_ON_PORT,MODEM_ON_PIN);
		set_timeout(800);
		while_timeout();
		if (send_command_to_GSM("AT","OK",gsm_message,1000,3000)) modem_state = MODEM_STATE_ON;
	}
}

char get_next_gsm_message(){
	int k = 0;
	while ((gsm_buffer[gsm_message_check_counter-2] != 13) & (gsm_message_check_counter != gsm_buffer_char_counter)){
		gsm_message[k] = gsm_buffer[gsm_message_check_counter];
		gsm_message_check_counter++;
		if (gsm_message_check_counter == GSM_BUFFER_SIZE) gsm_message_check_counter = 0;
		k++;
	}
	if (gsm_message[0]) return OK_ANSWER;
	else return NO_ANSWER;
}

void clear_gsm_buffer(){
	while (get_next_gsm_message() != 0){
	}
}

 char check_gsm_message(){
		while(get_next_gsm_message()){
			parse_gsm_message();
		}
	return 0;
}



char parse_gsm_message(){
	char gsm_message_number = UNKNOW_GSM_MESSAGE;
	if (find_str("OK\r\n",gsm_message)) gsm_message_number = OK_GSM_MESSAGE;
	if (find_str("ERROR\r\n",gsm_message)) gsm_message_number = ERROR_GSM_MESSAGE;
	gsm_buffer[gsm_message_check_counter-2] = 0;
	clear_gsm_message();
	return gsm_message_number;
}

void clear_gsm_message(){
	int i = 0;
	while ((gsm_message[i] != '\0') & (i<GSM_MESSAGE_SIZE)){
		gsm_message[i] = 0;
		i++;
	}
}


char send_command_to_GSM(char * s,char * await_ans, char * answer, int t, int max_t){

	send_string_to_GSM(s);
	send_char_to_GSM('\r');
	while(max_t > 0){
		set_timeout(t);
		while(!time_out);
		char c = get_next_gsm_message();
		if(c == NO_ANSWER){
			max_t -= t;
		}else{
			if (find_str("OK",answer)) return OK_ANSWER;
			else{
				max_t -= t;
				parse_gsm_message();
			}
		}
	}
	return NO_ANSWER;
}


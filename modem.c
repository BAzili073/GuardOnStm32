#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "EEPROMfunc.h"
#include "defines.h"
#include "TIMs.h"
#include "UART.h"
#include "guard_func.h"
#include "my_string.h"
#include "sms_command.h"
#include "modem.h"

void send_text_as_ucs(char * out_message, unsigned int len);
char parse_gsm_message();
void clear_gsm_message();
char check_gsm_message();
void del_all_gsm_message();

void modem_call(char * number);
char modem_send_sms_message(char * number,char * text);
char send_sms_message_for_all(char * text,int function);
char modem_setup();

void clear_output_sms_message();
char check_number_of_sms();

int modem_state = MODEM_STATE_NOT_NEED;
char get_next_gsm_message();
volatile unsigned int gsm_message_check_counter = 0;
char send_command_to_GSM(char * s,char * await_ans, char * answer, int t, int max_t);

void convert_number_to_upd(char * number);
unsigned int hexchar_to_dec(char a);
void send_int_as_hex_to_GSM(int x);
int modem_time_on = 0;

char gsm_message[GSM_MESSAGE_SIZE];
char output_sms_message[70];
int modem_action = MODEM_ACTION_FREE;
//char input_sms_message[70];
int gsm_signal_quality;
char modem_setup_ok = 0;
char modem_time_check = 0;
char incoming_rings = 0;
void modem_check_state(){
	if (modem_action == MODEM_ACTION_FREE){
		switch (modem_state){
			case MODEM_STATE_NOT_NEED:

			break;
			case MODEM_STATE_NO_SIM:

			break;
			case MODEM_STATE_OFF:
				MODEM_ON();
			break;
			case MODEM_STATE_ON:
				modem_check_online();
			break;
			case MODEM_STATE_OFFLINE:
				modem_check_online();
			break;
			case MODEM_STATE_SETUP:
				if (!modem_setup_ok){
					modem_setup_ok = modem_setup();
					if (modem_setup_ok) modem_state = MODEM_STATE_ONLINE;
				}
			break;
			case MODEM_STATE_ONLINE:
				modem_check_quality();
			break;
		}
	}


}
void MODEM_ON(){
	modem_setup_ok = 0;
	check_gsm_message();
	while(modem_state == MODEM_STATE_OFF){
		GPIO_LOW(MODEM_ON_PORT,MODEM_ON_PIN);
		set_timeout_7(10);
		while_timeout_7();
		GPIO_HIGH(MODEM_ON_PORT,MODEM_ON_PIN);
		set_timeout_7(20);
		while_timeout_7();
		if (send_command_to_GSM("AT","OK",gsm_message,10,30)){
			modem_state = MODEM_STATE_ON;
		}
	}
}

void MODEM_OFF(){
	check_gsm_message();
	while(modem_state != MODEM_STATE_NOT_NEED){
		GPIO_LOW(MODEM_ON_PORT,MODEM_ON_PIN);
		set_timeout_7(10);
		while_timeout_7();
		GPIO_HIGH(MODEM_ON_PORT,MODEM_ON_PIN);
		set_timeout_7(20);
		while_timeout_7();
		if (!send_command_to_GSM("AT","OK",gsm_message,2,16)){
			modem_state = MODEM_STATE_NOT_NEED;
		}
	}
}


void modem_check_online(){
	if (!modem_time_check) {
		modem_time_check = SET_MODEM_TIME_CHECK;
		send_string_to_GSM("AT+COPS?\r");
	}
}
void modem_check_quality(){
	if (!modem_time_check){
		modem_time_check = SET_MODEM_TIME_CHECK;
		send_string_to_GSM("AT+CSQ\r");
	}
}


char get_next_gsm_message(){
	int k = 0;
	while(1){
		if ((gsm_message_check_counter == gsm_buffer_char_counter)) {
			break;
		}else if((gsm_buffer[gsm_message_check_counter-1] == '>') && (gsm_buffer[gsm_message_check_counter] == ' ')){
			gsm_message_check_counter++;
			break;
		}else if((gsm_buffer[gsm_message_check_counter] == 10) || (gsm_buffer[gsm_message_check_counter] == 13)){
			while((gsm_buffer[gsm_message_check_counter] == 13) || (gsm_buffer[gsm_message_check_counter] == 10)){
				gsm_buffer[gsm_message_check_counter] = 0;
				gsm_message_check_counter++;
				if (gsm_message_check_counter == GSM_BUFFER_SIZE) gsm_message_check_counter = 0;
			}
			break;
		}else{
			if((gsm_buffer[gsm_message_check_counter-1] == 10) || (gsm_buffer[gsm_message_check_counter-1] == 13)){
//				c
//				gsm_buffer[gsm_message_check_counter-2] = 0;
				break;
			}
			gsm_message[k] = gsm_buffer[gsm_message_check_counter];
			gsm_message_check_counter++;
			if (gsm_message_check_counter == GSM_BUFFER_SIZE) gsm_message_check_counter = 0;
			k++;
		}
	}

	if (gsm_message[0]) return OK_ANSWER;
	else return NO_ANSWER;
}

 char check_gsm_message(){
		while(get_next_gsm_message()){
			parse_gsm_message();
		}
	return 0;
}



char parse_gsm_message(){
	int return_gsm_message = GSM_MESSAGE_UNKNOW;
	if (find_str("+CMTI:",gsm_message)){
		incoming_sms();
		return_gsm_message = GSM_MESSAGE_INCOMING_SMS;
	}else if (find_str("+CPIN:",gsm_message)){
		if (find_str("READY",gsm_message)){
//			modem_state = MODEM_STATE_NO_SIM;
//			return_gsm_message = GSM_MESSAGE_NO_SIM;
		}
		if (find_str("NOT INSERTED",gsm_message)){
			modem_state = MODEM_STATE_NO_SIM;
			return_gsm_message = GSM_MESSAGE_NO_SIM;
		}

	} else if (find_str("+COPS:",gsm_message)){
		if (str_length(gsm_message) > 15) {
			modem_state = MODEM_STATE_SETUP;
		}else{
			modem_state = MODEM_STATE_OFFLINE;
		}
	}else if (find_str("BUSY",gsm_message)){
			modem_free();
			return_gsm_message = GSM_MESSAGE_CALL_BUSY;
	}else if (find_str("NO ANSWER",gsm_message)){
				modem_free();
			return_gsm_message = GSM_MESSAGE_CALL_NO_ANSWER;
	}else if (find_str("+CSQ:",gsm_message)){
		if (gsm_message[7] == ','){
			gsm_signal_quality = gsm_message[6] - 48;
		}else{
			gsm_signal_quality = (gsm_message[6] - 48) * 10 + (gsm_message[7] - 48);
		}
	}else if(find_str("NO CARRIER",gsm_message)){
		modem_no_carrier();

	}else if(find_str("+CLIP:",gsm_message)){
		incoming_call();
	}else if (find_str("OK\r\n",gsm_message)){
		return_gsm_message = GSM_MESSAGE_OK;
	}else if (find_str("ERROR\r\n",gsm_message)){
		return_gsm_message = GSM_MESSAGE_ERROR;
	}
	clear_gsm_message();
	return return_gsm_message;
}


void incoming_call(){
	if (modem_action == MODEM_ACTION_FREE){
		int i;
			for (i = 0;i<10;i++){
				tel_number_temp[i] = gsm_message[i+11];
			}
			if (tel_number[0][8] != 'F'){
				modem_save_number(0,tel_number_temp);
			}
		modem_action = MODEM_ACTION_INCOMING_CALL;
	}
//	  incoming_rings ++;
//	if (incoming_rings == 4) {
//		send_string_to_GSM("ATA\r");
//		modem_action = MODEM_ACTION_TALK;
//	}
}


void incoming_sms(){
	send_command_to_GSM("AT+CMGR=1,0","+CMGR:",gsm_message,2,5);
	send_command_to_GSM("","0008",gsm_message,2,5);
//					get_next_gsm_message();
	parse_incoming_sms();
	send_command_to_GSM("AT+CMGD=1,4","OK",gsm_message,2,5);
}

void modem_free(){
	modem_action = MODEM_ACTION_FREE;
}

void modem_no_carrier(){
	if (modem_action == MODEM_ACTION_INCOMING_CALL){
		if (incoming_rings == 2){

		}
	}
	incoming_rings = 0;
	modem_free();
}


void clear_gsm_message(){
	int i = 0;
	while ((gsm_message[i] != 0) && (i<GSM_MESSAGE_SIZE)){
		gsm_message[i] = 0;
		i++;
	}
}


char send_command_to_GSM(char * s,char * await_ans, char * answer, int t_msec, int max_t){
	send_string_to_GSM(s);
	send_char_to_GSM('\r');
	while(max_t > 0){
		set_timeout_7(t_msec);
		while_timeout_7();
		char c = get_next_gsm_message();
		if(c == NO_ANSWER){
			max_t -= t_msec;
		}else{
			if (find_str(await_ans,answer)) return OK_ANSWER;
			else{
				max_t -= t_msec;
				if (parse_gsm_message() == GSM_MESSAGE_ERROR) return NO_ANSWER;;
			}
		}
	}
	return NO_ANSWER;
}
//#define MODEM_TEXT_MODE
char modem_setup(){
#ifdef MODEM_TEXT_MODE
	if (!send_command_to_GSM("AT+CMGF=1","OK",gsm_message,2,5)) return 0;
#else
	if (!send_command_to_GSM("AT+CMGF=0","OK",gsm_message,2,5)) return 0;
#endif

	if (!send_command_to_GSM("AT+CSCS=\"GSM\"","OK",gsm_message,2,5)) return 0;
	if (!send_command_to_GSM("AT+CLIP=1","OK",gsm_message,2,5)) return 0;   // for AON
	if (!send_command_to_GSM("AT+CPBS=\"SM\"","OK",gsm_message,2,5)) return 0;   // select sim as memory
	return 1;
}

char send_sms_message_for_all(char * text,int function){
	unsigned int i;
	for (i=0;i<MAX_TEL_NUMBERS;i++){
		if ((tel_access[i] == TEL_ACCESS_ADMIN) || (tel_access[i] == TEL_ACCESS_DOUBLE_SMS) || (tel_access[i] == function)){
			if (!modem_send_sms_message(tel_number[i],text)) return 0;
		}
	}
	clear_output_sms_message();
	return 1;
}

void clear_output_sms_message(){
	unsigned int i;
	for (i=0;i<70;i++) {
		output_sms_message[i] = 0;
	}
}



char modem_send_sms_message(char * number,char * text){
	if ((modem_state != MODEM_STATE_ONLINE) || (number[8] != 'F')){
		if (modem_state == MODEM_STATE_NOT_NEED){
				modem_state = MODEM_STATE_OFF;
		}
		return 0;
	}
#ifdef MODEM_TEXT_MODE
	send_string_to_GSM("AT+CMGS=\"+7");
	send_string_to_GSM(number);
	if (!send_command_to_GSM("\"",">",gsm_message,2,5)) return 0;
	send_string_to_GSM(text);
	send_char_to_GSM(0x1a);
#else
	send_string_to_GSM("AT+CMGS=");
	send_int_to_GSM(((str_length(text) * 2)+13));
	if (!send_command_to_GSM("",">",gsm_message,2,30)) return 0;
	 send_string_to_GSM("0001000B91");
	 send_string_to_GSM("97");
	 int i;
	 for (i = 0;i<10;i++) send_char_to_GSM(number[i]);
//	 send_string_to_GSM(number);//number);
	 send_string_to_GSM("0008");
	 send_int_as_hex_to_GSM((str_length(text) * 2));
//	 send_string_to_GSM("0C");
	 send_text_as_ucs(text,str_length(text));
//	 send_string_to_GSM("041F04400438043204350442");
//	 send_string_to_GSM(int_to_hex(str_length(text))); // send length text as HEX
//	 send_string_to_GSM(eng_to_ucs(text)); //send text as hex
	 send_char_to_GSM(0x1a);
//	 if (!send_command_to_GSM(0x1a,">",gsm_message,2,12)) return 0;
//	 if (!send_command_to_GSM("\x1a",">",gsm_message,2,12)) return 0;
	 if (!send_command_to_GSM("","+CMGS:",gsm_message,2,50)) {
		 return 0;
	 }
#endif
	return 1;
}
void send_int_as_hex_to_GSM(int x){
	char a;
	a = x / 16;
	if (a>9) a = 'A' + (a - 10);
	else a = '0' + a;
	send_char_to_GSM(a);
	a = x - (a*16);
	if (a>9) a = 'A' + (a - 10);
	else a = '0' + a;
	send_char_to_GSM(a);
}

void int_to_hex(int x){

}


void ucs_to_eng(char * in_message, char * message){
	unsigned int i;
	char abc[] = "abvgdejziIklmnoprstufhcCwWDQBEUq";
	unsigned int len;
	if (in_message[53] > 64) len = 10 + 'A' - in_message[53];
	else len = in_message[53]- '0';
	len = (((in_message[52] - '0')*16 + len)*4);
//	char sign[] = " !{034}#$%&'()*+,-./0123456789:;<=>?";
	for (i = 54;i<(54 + len);i=i+4){
		if (in_message[i+1] == '4'){
			if (in_message[i+2] == '1' || in_message[i+2] == '3'){
				message[((i - 54)/4)] = abc[(hexchar_to_dec(in_message[i+3]))];
			}else if(in_message[i+2] == '2' || in_message[i+2] == '4'){
				message[((i - 54)/4)] = abc[(hexchar_to_dec(in_message[i+3]) + 16)];
			}
		}else if (in_message[i+1] == '0'){
			message[((i - 54)/4)] = (hexchar_to_dec(in_message[i+2])<<4) + (hexchar_to_dec(in_message[i+3]));
		}
	}
}

void send_text_as_ucs(char * out_message, unsigned int len){
	char abc[] = "abvgdejziIklmnoprstufhcCwWDQBEUq";
	char sign[] = " !\"#$%&'()*+,-./0123456789:;<=>?";
	unsigned int i = 0;
	unsigned int y = 0;
	for (i = 0;i<len;i++){
		if (out_message[i] < 65){
				send_string_to_GSM("00");
				for (y = 0;y<33;y++){
					if (out_message[i] == sign[y]){
						send_int_as_hex_to_GSM((32 + y));
						break;
					}
				}
		}else{
			send_string_to_GSM("04");
			for (y = 0;y<33;y++){
				if (out_message[i] == abc[y]){
					send_int_as_hex_to_GSM((48 + y));
					break;
				}
			}
		}
	}
}

unsigned int hexchar_to_dec(char a){
	if ((a - '0' < 10)) return (a - '0');
	else return (a-'A'+10);
}

void convert_number_to_upd(char * number){
	number[9] = 'F';
	unsigned int i = 0;
	char temp;
	for (i=0; i<10; i=i+2){
		temp = number[i];
		number[i] = number[i+1];
		number[i+1] = temp;
	}
}

void convert_number_to_eng(char * number){
	unsigned int i = 0;
	char temp;
	for (i=0; i<10; i=i+2){
		temp = number[i];
		number[i] = number[i+1];
		number[i+1] = temp;
	}
	number[9] = 0;
}

void modem_call(char * number){
	if (modem_action == MODEM_ACTION_FREE){
		send_string_to_GSM("ATD+79");
		send_string_to_GSM(number);
		send_string_to_GSM(";\n\r");
		modem_action = MODEM_ACTION_OUTGOING_CALL;
	}
}


char check_number_of_sms(){
	unsigned int i = 0;
	unsigned int y = 0;
	char access;
	for (y = 0;y<MAX_TEL_NUMBERS;y++){
		access = 1;
		for (i = 24;i < 34;i++){
			if (tel_number[y][i-24] != gsm_message[i]){
				access = 0;
				break;
			}
		}
		if ((tel_access[y] == TEL_ACCESS_ADMIN) && access) return y;
	}
	return TEL_NUMBER_DENY;
}
/*AT COMMANDS
 AT+COPS?   GET OPERATOR
 AT+CSQ     GET SIGNAL QUALITY

 */

void modem_save_number(char ID_number,char * number){
	convert_number_to_upd(number);
	int i;
	for (i = 0;i<10;i++){
		tel_number[ID_number][i] = number[i];
		EEPROMWrite((EEPROM_tel_numbers + ID_number*10+i),number[i],1);
	}
}

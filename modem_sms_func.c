#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "modem_module.h"
#include "sms_command.h"
#include "modem.h"
//#include "sms_command.h"
extern TEL_obj tel[MAX_TEL_NUMBERS];
extern int modem_state;
void incoming_sms(){
	send_command_to_GSM("AT+CMGR=1,0","+CMGR:",gsm_message,2,5);
	send_command_to_GSM("","0008",gsm_message,2,5);
	send_command_to_GSM("AT+CMGD=1,4","OK",gsm_message,2,0);
//					get_next_gsm_message();
	parse_incoming_sms();

}

char send_sms_message_for_all(char * text,int function){
	unsigned int i;
	for (i=0;i<MAX_TEL_NUMBERS;i++){
		if ((tel[i].access == TEL_ACCESS_ADMIN) || (tel[i].access == TEL_ACCESS_DOUBLE_SMS) || (tel[i].access == function)){
			if (!modem_send_sms_message(tel[i].number,text));// return 0;
		}
	}
	clear_output_sms_message();

	return 1;
}


char modem_send_sms_message(char * number,char * text){

	if ((modem_errors[MODEM_ERRORS_SEND_SMS] > 3)){
		modem_call(number);
		return 0;
	}
	if ((modem_state != MODEM_STATE_ONLINE) || (number[0] == 0) || (modem_action != MODEM_ACTION_FREE)){
		if (modem_state == MODEM_STATE_NOT_NEED){
				modem_state = MODEM_STATE_OFF;
		}
		return 0;
	}

	led_blink(OUT_MODE_GSM,5,5);
	modem_action = MODEM_ACTION_SMS;
#ifdef MODEM_TEXT_MODE
	send_string_to_GSM("AT+CMGS=\"+7");
	send_string_to_GSM(number);
	if (!send_command_to_GSM("\"",">",gsm_message,2,5)) return 0;
	send_string_to_GSM(text);
	send_char_to_GSM(0x1a);
#else
#ifdef DEBUG_MODEM
	send_string_to_UART3("MODEM: SEND SMS! NUMBER:  ");
	send_string_to_UART3(number);
	send_string_to_UART3(" TEXT: ");
		send_string_to_UART3(text);
	send_string_to_UART3(" \n\r");
#endif
	send_string_to_GSM("AT+CMGS=");
	send_int_to_GSM(((str_length(text) * 2)+13));
	if (!send_command_to_GSM("",">",gsm_message,2,80)){
		modem_free();
		return 0;
	}
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
	 if (!send_command_to_GSM("","+CMGS:",gsm_message,5,200)) {

#ifdef DEBUG_MODEM
	send_string_to_UART3("MODEM: SEND SMS ERROR!");
	send_string_to_UART3(" \n\r");
#endif

		 modem_free();
		 modem_errors[MODEM_ERRORS_SEND_SMS]++;
		 return 0;
	 }
#endif
#ifdef DEBUG_MODEM
	send_string_to_UART3("MODEM: SEND SMS OK! \n\r");
#endif
	modem_errors[MODEM_ERRORS_SEND_SMS] = 0;
	 modem_free();
	return 1;
}

void sms_ucs_to_eng(char * in_message, char * message){
	unsigned int len;
	if (in_message[53] > 64) len = (in_message[53] - 55)/2;
	else len = in_message[53]- '0';
	len = (((in_message[52] - '0')*16 + len)*4);
	ucs_to_eng(in_message, message,len,54);

//	char sign[] = " !{034}#$%&'()*+,-./0123456789:;<=>?";
}

char get_next_command_from_sms(char * sms, char * command, char num_cmd){
	char i;
	char command_char = 0;
	char n_count = 0;
	for (i = 0;i<70;i++){
		if (sms[i] == '\n' || sms[i] == 0){
			if (n_count == num_cmd){
				for (;command_char<70;command_char++){
					command[command_char] = 0;
				}
				if (command[0]) return 1;
				else return 0;
			}
			command_char = 0;
			n_count++;
		}else{
			command [command_char] = sms[i];
			command_char++;
		}
	}
	return 0;
}

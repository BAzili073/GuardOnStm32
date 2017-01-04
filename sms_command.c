#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "EEPROMfunc.h"
#include "defines.h"
#include "TIMs.h"
#include "UART.h"
#include "guard_func.h"
#include "my_string.h"
#include "modem_module.h"
#include "modem.h"
#include "guard_func.h"
#include "FingerPrint.h"
#include "input.h"

extern TEL_obj tel[MAX_TEL_NUMBERS];


void sms_command_nn();
void sms_command_r();

void parse_incoming_sms(){
	uint8_t temp = 0;
	uint8_t temp2 = 0;
	uint8_t temp3 = 0;
	int i;
	for (i = 0;i<11;i++) tel_number_temp[i] = gsm_message[i+24];
	last_control_ID_number = check_number(tel_number_temp);

	if (last_control_ID_number > MAX_TEL_NUMBERS) return;
	ucs_to_eng(gsm_message, input_sms_message);
#ifdef DEBUG_MODEM
	send_string_to_UART3("INCOMING SMS! NUMBER: ");
	send_string_to_UART3(tel_number_temp);
	send_string_to_UART3(" ID: ");
	send_int_to_UART3(last_control_ID_number);
	send_string_to_UART3(" \n\r");
	send_string_to_UART3(" SMS: ");
	send_string_to_UART3(input_sms_message);
	send_string_to_UART3(" \n\r");
#endif

	switch(input_sms_message[0]){
//nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
		case 'n':
			switch(input_sms_message[1]){
				case 'n':
					sms_command_nn();
				break;
				case 't':
					set_ds18x20_settings((input_sms_message[2]-'0'),parse_int_in_message(input_sms_message,4),parse_int_in_message(input_sms_message,8),parse_int_in_message(input_sms_message,12));
				break;
				case 'v':
					temp = 0;
					if (input_sms_message[2] == 't'){
						set_input_text((input_sms_message[3]-'0'),input_sms_message);
					}else{
						for (i = 0;;i++) {
						if ((input_sms_message[6 + i] != '0') && (input_sms_message[6 + i] != '1')) break;
							temp = (temp<<1) | (input_sms_message [6 + i] - '0');
						}
						set_input_settings((input_sms_message[2]-'0'),(input_sms_message[5]-'0'),(input_sms_message[4]-'0'),temp,parse_int_in_message(input_sms_message,(6+i+1)));
					}

				break;
				case 'r':
					for (i = 0;i < MAX_OUTPUT;i++){
						set_output_settings(i,(input_sms_message[3 + i]));
					}
				break;
				case 'i':
					for (i = 0;i < MAX_LED;i++){
						set_led_settings(i,(input_sms_message[3 + i]));
					}
				break;
				case 's':
					temp = 0;
					temp2 = 0;
					temp3 = 0;
					for (i = 0;;i++) {
						if ((input_sms_message[3 + i] != '0') && (input_sms_message[3 + i] != '1')) break;
						temp = (temp<<1) | (input_sms_message[3 + i] - '0');
					}
					temp2 = parse_int_in_message(input_sms_message,(3+i+1));
					temp3 = parse_int_in_message(input_sms_message,(3+i+1+ get_size_number(temp2) +1));
					set_device_setting(temp,temp2,temp3);

				break;

			}
		break;
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
		case 'o':
				str_add_str(last_control_guard,sizeof(last_control_guard),"+79",0);
				str_add_str(last_control_guard,sizeof(last_control_guard),tel[last_control_ID_number].number,10);
				if((input_sms_message[1] - '0')){
					guard_on();
				}else{
					guard_off();
				}
		break;
//rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
		case 'r':
				sms_command_r();
		break;
//
	}
	for (i = 0;i<70;i++) input_sms_message[i] = 0;

}


void sms_command_nn(){

	unsigned int ID_number  = (input_sms_message[3] - '0');
	char row_number[10];
	unsigned int i;
	for (i = 0;i<10;i++){
		row_number[i] = input_sms_message[8+i];
	}
	convert_number_to_upd(row_number);
	uint8_t acc = input_sms_message[18] - '0';
	modem_save_number(ID_number,row_number,acc);
}

void sms_command_r(){
		char a[1];
		unsigned int i;
		str_add_str(output_sms_message,sizeof(output_sms_message),(get_guard_st() ? "na ohrane" : "snqt s ohranQ "),0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"vh:",0);
		for (i = 1;i<6;i++) {
			a[0] = (('-' - (check_input(i)*2)));
			str_add_str(output_sms_message,sizeof(output_sms_message),a,MAX_INPUT);
		}
		str_add_str(output_sms_message,sizeof(output_sms_message)," ",0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"vQh:",0);
		for (i = 1;i<6;i++) {
		//	a[0] = ('1' - (GPIO_READ((outputs_port[(i-1)]),(output[i-1].pin))*2));
			str_add_str(output_sms_message,sizeof(output_sms_message),a,MAX_OUTPUT);
		}
		str_add_str(output_sms_message,sizeof(output_sms_message)," ",0);
		send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);
}

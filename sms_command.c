#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "EEPROMfunc.h"
#include "defines.h"
#include "TIMs.h"
#include "UART.h"
#include "guard_func.h"
#include "my_string.h"
#include "modem.h"
#include "guard_func.h"
#include "FingerPrint.h"

void sms_command_nn();
void sms_command_r();

void parse_incoming_sms(){

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
			if (input_sms_message[1] == 'n') {
				sms_command_nn();
			}
		break;
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
		case 'o':
				str_add_str(last_control_guard,"+79");
				char number_x[10];
				str_add_str(number_x,tel_number[last_control_ID_number]);
				str_add_str(last_control_guard,number_x);
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

	}
	for (i = 0;i<70;i++)	input_sms_message[i] = 0;

}


void sms_command_nn(){
	unsigned int ID_number  = (input_sms_message[3] - '0');
	char row_number[10];
	unsigned int i;
	for (i = 0;i<10;i++){
		row_number[i] = input_sms_message[8+i];

	}
		modem_save_number(ID_number,row_number);
}
void sms_command_r(){
		char a[1];
		unsigned int i;
		str_add_str(output_sms_message,(guard_st ? "na ohrane" : "snqt s ohranQ "));
		str_add_str(output_sms_message,"vh:");
		for (i = 1;i<6;i++) {
			a[0] = (('-' - (check_input(i)*2)));
			str_add_str(output_sms_message,a);
		}
		str_add_str(output_sms_message," ");
		str_add_str(output_sms_message,"vQh:");
		for (i = 1;i<6;i++) {
			a[0] = ('1' - (GPIO_READ((outputs_port[(i-1)]),(outputs[i-1]))*2));
			str_add_str(output_sms_message,a);
		}
		str_add_str(output_sms_message," ");
		send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);
}

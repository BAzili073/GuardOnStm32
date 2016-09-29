#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "EEPROMfunc.h"
#include "defines.h"
#include "TIMs.h"
#include "UART.h"
#include "guard_func.h"
#include "my_string.h"
#include "modem.h"

void sms_command_nn();

void parse_incoming_sms(){
	last_control_ID_number = check_number_of_sms();
	if (last_control_ID_number > MAX_TEL_NUMBERS) return;
	ucs_to_eng(gsm_message, input_sms_message);
	if (input_sms_message[0] == 'n') {
		switch(input_sms_message[1]){
			case 'n':
				sms_command_nn();
			break;
		}
	}else if((input_sms_message[0] == 'o')){
		str_add_str(last_control_guard,"+79");
		char number_x[10];
		str_add_str(number_x,tel_number[last_control_ID_number]);
		str_add_str(last_control_guard,number_x);
		if((input_sms_message[1] - '0')){
			guard_on();
		}else{
			guard_off();
		}
	}else if((input_sms_message[0] == 'r')){
		sms_command_r();
	}
}


void sms_command_nn(){
	unsigned int ID_number  = (input_sms_message[3] - '0');
	char row_number[10];
	unsigned int i;
	for (i = 0;i<10;i++){
		row_number[i] = input_sms_message[8+i];

	}
		convert_number_to_upd(row_number);
		for (i = 0;i<10;i++){
			tel_number[ID_number][i] = row_number[i];
			EEPROMWrite((EEPROM_tel_numbers + ID_number*10+i),row_number[i],1);
		}

}
void sms_command_r(){
		char a[1];
		unsigned int i;
		str_add_str(output_sms_message,(guard_st ? "na ograne" : "snqt s ohranQ "));
		str_add_str(output_sms_message,"vh:");
		for (i = 1;i<6;i++) {
			a[0] = (('-' - (check_input(i)*2)));
			str_add_str(output_sms_message,a);
		}
		str_add_str(output_sms_message," ");
		str_add_str(output_sms_message,"vQh:");
		for (i = 1;i<6;i++) {
			a[0] = ('1' - (GPIO_READ(OUTPUT_PORT,(outputs[i-1]))*2));
			str_add_str(output_sms_message,a);
		}
		str_add_str(output_sms_message," ");
		send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);
}

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
#include "DS18x20.h"
#include "led.h"
#include "output.h"

extern TEL_obj tel[MAX_TEL_NUMBERS];


void sms_command_nn(char * command_str);
void sms_command_r();
extern char tel_number_temp[10];

void parse_incoming_sms(){
	uint8_t temp = 0;
	int32_t temp2 = 0;
	int32_t temp3 = 0;
	int i;
	char command_str[70];
	char command[20];
	char command_count = 0;
	char bug_command_count = 0;
	for (i = 0;i<11;i++){
		tel_number_temp[i] = gsm_message[i+24];
	}
	last_control_ID_number = check_number(tel_number_temp);

	if (last_control_ID_number > MAX_TEL_NUMBERS) return;
	sms_ucs_to_eng(gsm_message, input_sms_message);
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
	while(get_next_command_from_sms(input_sms_message,command_str,command_count)){
		command_count++;
#ifdef DEBUG_MODEM
	send_string_to_UART3("Progessing command: ");
	send_string_to_UART3(command_str);
	send_string_to_UART3("\n\r");
#endif
		switch(command_str[0]){
	//nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
			case 'n':
				switch(command_str[1]){
					case 'n':
						//nnX +79021201364
						sms_command_nn(command_str);

					break;
					case 't':
						//ntX -10,29,00010011
						temp = 0;
						temp2 = parse_int_in_message(command_str,4);
						temp3 = parse_int_in_message(command_str,(4 + get_size_number(temp2) + 1));
						for (i = 0;;i++) {
							char a = command_str[4 + i + get_size_number(temp2) + 1 + get_size_number(temp3) +1];
							if ((a != '0') && (a != '1')) break;
								temp = (temp<<1) | (a - '0');
						}
						set_ds18x20_settings((command_str[2]-'0'),temp2,temp3,temp);
					break;
					case 'v':
						temp = 0;
						if (command_str[2] == 't'){
							if (command_str[3] < '0' || command_str[3] > '9') break;
							//nvt1 dverb zapili
							set_input_text((command_str[3]-'0'),command_str);
						}else{
							if (command_str[2] < '0' || command_str[2] > '9') break;
							//nv1 3,7,000,2
							for (i = 0;;i++) {
							if ((command_str[8 + i] != '0') && (command_str[8 + i] != '1')) break;
								temp = (temp<<1) | (command_str [8 + i] - '0');
							}
							set_input_settings(
										(command_str[2]-'0'),
										(command_str[4]-'0'),
										(command_str[6]-'0'),
										temp,
										parse_int_in_message(command_str,(8+i+1))
									);
						}

					break;
					case 'r':
						//nr lov12
						for (i = 0;i < MAX_OUTPUT;i++){
							set_output_settings(i,(command_str[3 + i]));
						}
					break;
					case 'i':
						//ni lov12
						for (i = 0;i < MAX_LED;i++){
							set_led_settings(i,(command_str[3 + i]));
						}
					break;
					case 'p':
						//np 93
						setting_powered(1);
					break;
					case 'a':
						//np 93
						setting_powered(2);
					break;
					case 's':
						//ns
						temp = 0;
						temp2 = 0;
						temp3 = 0;
						for (i = 0;;i++) {
							if ((command_str[3 + i] != '0') && (command_str[3 + i] != '1')) break;
							temp = (temp<<1) | (command_str[3 + i] - '0');
						}
						temp2 = parse_int_in_message(command_str,(3+i+1));
						temp3 = parse_int_in_message(command_str,(3+i+1+ get_size_number(temp2) +1));
						set_device_setting(temp,temp2,temp3);

					break;
					default:
						bug_command_count++;
					break;

				}
			break;
	//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
			case 'o':
				//o0 o1 o
					str_add_str(last_control_guard,sizeof(last_control_guard),"+79",0);
					convert_number_to_eng(tel_number_temp);
					str_add_str(last_control_guard,sizeof(last_control_guard),tel_number_temp,10);
					if (command_str[1]) set_new_guard_st((command_str[1] - '0'));
					else set_new_guard_st(!get_guard_st());

			break;
	//rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
			case 'r':
				//r
					sms_command_r();
			break;
	//bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
			case 'b':
				//b *102#

				temp = str_length(command_str);
				for (i = 0; i < (temp - 2);i++){
					command[i] = command_str[i+2];
				}
				command[i+1] = 0;
				send_string_to_GSM("AT+CUSD=1,\"");
				send_string_to_GSM(command);
				send_command_to_GSM("\"","OK",gsm_message,2,5);
	//			send_command_to_GSM("AT+CUSD=1,\"*102#\"","OK",gsm_message,2,5);
			break;
	//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
			case 'a':
				if ((command_str[1]) == 's') {
					//vs 3
					if (!command_str[3]) FULL_RESET();
					temp = parse_int_in_message(command_str,(3));
					set_time_to_reset(temp);
				}else if ((command_str[1]) == 'r'){
					//vr 48
					setting_time_to_report(parse_int_in_message(command_str,(3)));
				}else{
					bug_command_count++;
				}

			break;
	//UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
			case 'u':
				if ((command_str[1]) == 'v') {
					//vs 3
					temp2 = parse_int_in_message(command_str,4);
					sms_control_output((command_str[2] - '0'), temp2);
				}else{
					bug_command_count++;
				}

			break;
//SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
			case 's':
				if ((command_str[1]) == 'b')
					if ((command_str[2]) == 'r')
						if ((command_str[3]) == 'o')
							if ((command_str[4]) == 's')
								FULL_ERASE_DEVICE();

			break;
			default:
				bug_command_count++;
			break;
		}

		for (i = 0;i<70;i++){
			if (!command_str[i]) break;
			command_str[i] = 0;
		}

	}

	if (check_device_setting(DEVICE_SETTING_SMS_AT_SMS_COMMAND)){
			if ((command_count - bug_command_count)){
				str_add_str(output_sms_message,sizeof(output_sms_message),"prinqto komand: ",0);
				str_add_num(output_sms_message,(command_count - bug_command_count));
			}
			if (bug_command_count){
					str_add_str(output_sms_message,sizeof(output_sms_message)," \nnevernaq komanda: ",0);
					str_add_num(output_sms_message,bug_command_count);
			}
			send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);
		}
	for (i = 0;i<70;i++) input_sms_message[i] = 0;
}


void sms_command_nn(char * command_str){
	if (command_str[2] < '0' || command_str[2] > '9')  return;
	unsigned int ID_number  = (command_str[2] - '0');
	char row_number[10];
	unsigned int i;
	for (i = 0;i<10;i++){
		row_number[i] = command_str[7+i];
	}
	convert_number_to_upd(row_number);
	uint8_t acc = command_str[17];
	if ((acc > 47) & (acc < 58)) acc = acc - '0';
	modem_save_number(ID_number,row_number,acc);
}

void sms_command_r(){
		char a[1];
		unsigned int i;
		str_add_str(output_sms_message,sizeof(output_sms_message),(get_guard_st() ? "na ohrane" : "snqt s ohranQ"),0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"\n",0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"vh:",0);
		for (i = 1;i<6;i++) {
			a[0] = (('-' - (check_input(i)*2)));
			str_add_str(output_sms_message,sizeof(output_sms_message),a,1);
		}
		str_add_str(output_sms_message,sizeof(output_sms_message),"\n",0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"vQh:",0);
		for (i = 1;i<6;i++) {
		//	a[0] = ('1' - (GPIO_READ((outputs_port[(i-1)]),(output[i-1].pin))*2));
			str_add_str(output_sms_message,sizeof(output_sms_message),a,1);
		}
		str_add_str(output_sms_message,sizeof(output_sms_message),"\n",0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"temp:",0);
		for (i = 0;i < get_DS18x20_count();i++){
			int16_t tp = get_last_temp_DS18x20_by_number(i);
			if (tp == ONE_WIRE_CONVERSION_ERROR) str_add_str(output_sms_message,sizeof(output_sms_message),"?",0);
			else str_add_num(output_sms_message,tp);
			str_add_str(output_sms_message,sizeof(output_sms_message),";",0);
		}
		str_add_str(output_sms_message,sizeof(output_sms_message),"\n",0);
		if (get_powered() == POWERED_220V) str_add_str(output_sms_message,sizeof(output_sms_message),"220v",0);
		else str_add_str(output_sms_message,sizeof(output_sms_message),"akkum!",0);
		send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);
}

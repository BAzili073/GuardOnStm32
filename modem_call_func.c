#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "modem_module.h"
#include "guard_func.h"
#include "modem.h"

char incoming_rings = 0;
extern TEL_obj tel[MAX_TEL_NUMBERS];

void modem_call(char * number){
	if (modem_errors[MODEM_ERRORS_NO_CARRIER] > 3){
		return;
	}
	if (modem_action == MODEM_ACTION_FREE){
		int i;
		for (i=0;i<11;i++){
			tel_number_temp[i] = number[i];
		}
		convert_number_to_eng(tel_number_temp);
		send_string_to_GSM("ATD+79");
		send_string_to_GSM(tel_number_temp);
		send_string_to_GSM(";\n\r");
		modem_action = MODEM_ACTION_OUTGOING_CALL;

#ifdef DEBUG_MODEM
	send_string_to_UART3("MODEM : CALL! Number:  ");
	send_string_to_UART3("+79");
	send_string_to_UART3(tel_number_temp);
	send_string_to_UART3(" \n\r");
#endif

	}
}

void incoming_call(){
	if (modem_action == MODEM_ACTION_FREE){
		led_blink(OUT_MODE_GSM,8,2);
		modem_action = MODEM_ACTION_INCOMING_CALL;
		int i;
			for (i = 0;i<10;i++){
				tel_number_temp[i] = gsm_message[i+11];
			}
			convert_number_to_upd(tel_number_temp);
			last_control_ID_number = check_number(tel_number_temp);

#ifdef DEBUG_MODEM
	send_string_to_UART3("INCOMING CALL! NUMBER: ");
	send_string_to_UART3(tel_number_temp);
	send_string_to_UART3(" ID: ");
	send_int_to_UART3(last_control_ID_number);
	send_string_to_UART3(" \n\r");
#endif

			if (last_control_ID_number > MAX_TEL_NUMBERS){
				if (send_command_to_GSM("ATH0","OK",gsm_message,2,50)){
					modem_free();
				}
				if (tel[0].number[0] == 0xFE){ ////////////
					modem_save_number(0,tel_number_temp,9);
					modem_send_sms_message(tel[0].number,"vaw nomer dobavlen v sistemu");
				}
				modem_free();
			}

	}

	  incoming_rings ++;
	if (incoming_rings == 4) {
		send_string_to_GSM("ATA\r");
		modem_action = MODEM_ACTION_TALK;
	}
}

void modem_no_carrier(){
	if ((modem_action == MODEM_ACTION_INCOMING_CALL) & (last_control_ID_number < MAX_TEL_NUMBERS)){
		if (incoming_rings < 4){
			str_add_str(last_control_guard,sizeof(last_control_guard),"+79",0);
			str_add_str(last_control_guard,sizeof(last_control_guard),convert_number_to_eng(tel_number_temp),10);
			get_guard_st() ? set_new_guard_st(0) : set_new_guard_st(1);
		}
	}else if (modem_action == MODEM_ACTION_OUTGOING_CALL){
		modem_errors[MODEM_ERRORS_NO_CARRIER]++;
	}
	incoming_rings = 0;
	modem_free();
}


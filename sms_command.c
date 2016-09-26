#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "EEPROMfunc.h"
#include "defines.h"
#include "TIMs.h"
#include "UART.h"
#include "guard_func.h"
#include "my_string.h"
#include "modem.h"

void parse_incoming_sms(){
	last_control_ID_number = check_number_of_sms();
	if (last_control_ID_number > MAX_TEL_NUMBERS) return;
	ucs_to_eng(gsm_message, sms_message);
	if (find_str("nn ",sms_message)) sms_command_nn();
}

void sms_command_nn(){
	char ID_number;
	char row_number[10];
	unsigned int i;
	for (i = 0;i<10;i++){
		row_number[i] = sms_message[8+i];

	}
		convert_number_to_upd(row_number);
		for (i = 0;i<10;i++){
			tel_number[(sms_message[3] - '0')][i] = row_number[i];
		}

}
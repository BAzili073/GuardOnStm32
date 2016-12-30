#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "modem_module.h"
#include "sms_command.h"
//#include "sms_command.h"

void incoming_sms(){
	send_command_to_GSM("AT+CMGR=1,0","+CMGR:",gsm_message,2,5);
	send_command_to_GSM("","0008",gsm_message,2,5);
//					get_next_gsm_message();
	parse_incoming_sms();
	send_command_to_GSM("AT+CMGD=1,4","OK",gsm_message,2,5);
}

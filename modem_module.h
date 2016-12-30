#ifdef MODEM_CALL_H
#else
#define MODEM_CALL_H

#include "defines.h"

char output_sms_message[70];
char input_sms_message[70];
char gsm_message[GSM_MESSAGE_SIZE];

void modem_time();



#endif

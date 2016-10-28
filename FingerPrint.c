#include "FingerPrint.h"
#include "UART.h"

#define FP_CMD_FINGER_PRINT 0x21


#define FP_RESPONSE_NO 0
#define FP_RESPONSE_FINGER_DETECT 1
#define FP_RESPONSE_FINGER_NOT_DETECT 2




const char FP_FINGER_DETECT[26] = 		{0x55,0xAA,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x01};
char FP_TEST_CONNECTION[26] = 		{0x55,0xAA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
char FP_GET_IMAGE[26] = 			{0x55,0xAA,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x01};
char FP_AFM_GO_TO_STANDBYMODE[26] = {0x55,0xAA,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x01};
char FP_GENERATE[26]  = 			{0x55,0xAA,0x00,0x00,0x60,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x01};
char FP__MERGE[26]= 				{0x55,0xAA,0x00,0x00,0x61,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x01};
char FP_SEARCH[26] = 				{0x55,0xAA,0x00,0x00,0x63,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x02};
char FP_GET_EMPTY_ID[26] = 			{0x55,0xAA,0x00,0x00,0x45,0x00,0x04,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0x01};
char  FP_GET_ENROLL_COUNT[26] =		{0x55,0xAA,0x00,0x00,0x48,0x00,0x04,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6,0x01};
char FP_DEL_CHAR[26] = 				{0x55,0xAA,0x00,0x00,0x44,0x00,0x04,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF2,0x01};
char FP_GENERATE_FOR_ENROLL[26] = 	{0x55,0xAA,0x00,0x00,0x60,0x00,0x02,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x01};



char FP_check(){
	if (UART2_get_next_data()){
		return FP_parse_data();
	}
	return 0;
}

int FP_parse_data(){
	switch(UART2_message[4]){

	case FP_CMD_FINGER_PRINT:
		if (UART2_message[10]){
			return FP_RESPONSE_FINGER_DETECT;
		}else{
			return FP_RESPONSE_FINGER_NOT_DETECT;
		}
	break;
	}
	UART2_clear_message();
	return FP_RESPONSE_NO;
}

void send_command_to_FP(char * cmd){
	int i;
	for (i = 0;i<26;i++){
		send_char_to_UART2(cmd[i]);
	}
}



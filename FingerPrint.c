#include "FingerPrint.h"
#include "UART.h"
#include "modem.h"
#include "guard_func.h"

#define FP_CMD_RESPONSE_DETECT_FINGER 0x21
#define FP_CMD_RESPONSE_SEARCH 0x63
#define FP_CMD_RESPONSE_GET_IMAGE 0x20
#define FP_CMD_RESPONSE_GENERATE 0x60
#define FP_CMD_RESPONSE_MERGE 0x61
#define FP_CMD_RESPONSE_GET_EMPTE_ID 0x45
#define FP_CMD_RESPONSE_STORE_CHAR 0x40
#define FP_CMD_RESPONSE_DELETE 0x44


#define FP_RESPONSE_NO 0
#define FP_RESPONSE_FINGER_DETECT 1
#define FP_RESPONSE_FINGER_NOT_DETECT 2
#define FP_RESPONSE_FINGER_IN_BASE 3
#define FP_RESPONSE_FINGER_NOT_IN_BASE 4
#define FP_RESPONSE_GENERATE 5
#define FP_RESPONSE_GET_IMAGE 6
#define FP_RESPONSE_GET_EMPTY_ID 7
#define FP_RESPONSE_STORE_SUCC 8
#define FP_RESPONSE_DELETE_SUCC 9

#define FP_STEP_OFF 0
#define FP_STEP_WAIT 1
#define FP_STEP_DETETCT_FINGER 2
#define FP_STEP_GET_IMAGE 3
#define FP_STEP_GENERATE 4
#define FP_STEP_SEARCH_IN_BASE 5
#define FP_STEP_INPUT 6
#define FP_STEP_MERGE 7
#define FP_STEP_STORE_CHAR 8
#define FP_STEP_GET_EMPTY_ID 9

#define FP_MODE_MAIN 1
#define FP_MODE_REC 2
#define FP_MODE_DEL 3

int FP_detect_time;
int FP_current_step = FP_STEP_DETETCT_FINGER;
int FP_current_mode = FP_MODE_MAIN;
char empty_id[2];
char generate_for_enroll_step = 1;
char FP_try = 0;
int FP_time_check = 2;
char FP_check_allow;
char FP_time_for_rec = 0;
char FP_del_base = 0;

const char FP_CMD_FINGER_DETECT[26] = 	{0x55,0xAA,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x01};
char FP_CMD_TEST_CONNECTION[26] = 		{0x55,0xAA,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
char FP_CMD_GET_IMAGE[26] = 			{0x55,0xAA,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x01};
char FP_CMD_AFM_GO_TO_STANDBYMODE[26] = {0x55,0xAA,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0x01};
char FP_CMD_GENERATE[26]  = 			{0x55,0xAA,0x00,0x00,0x60,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x01};
char FP_CMD_MERGE[26]= 					{0x55,0xAA,0x00,0x00,0x61,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x01};
char FP_CMD_SEARCH[26] = 				{0x55,0xAA,0x00,0x00,0x63,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x02};
char FP_CMD_GET_EMPTY_ID[26] = 			{0x55,0xAA,0x00,0x00,0x45,0x00,0x04,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0x01};
char FP_CMD_GET_ENROLL_COUNT[26] =		{0x55,0xAA,0x00,0x00,0x48,0x00,0x04,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6,0x01};
char FP_CMD_DEL_CHAR[26] = 				{0x55,0xAA,0x00,0x00,0x44,0x00,0x04,0x00,0x01,0x00,0xA4,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF2,0x01};
char FP_CMD_GENERATE_FOR_ENROLL[26] = 	{0x55,0xAA,0x00,0x00,0x60,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x61,0x01};
char FP_CMD_SET_PARAM[26] = 			{0x55,0xAA,0x00,0x00,0x02,0x00,0x05,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x01};
char FP_CMD_STORE_CHAR[26] = 			{0x55,0xAA,0x00,0x00,0x40,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};



#define ALARM_FLAG_TEMPERATURE 0
#define ALARM_FLAG_ACC 1
#define ALARM_FLAG_FP_TRY 2

char FP_check(){
	if (UART2_get_next_data()){
		int response = FP_parse_data();

/////////////          FINGER NOT IN BASE
		if ((response == FP_RESPONSE_FINGER_NOT_IN_BASE)){
			FP_detect_time = 20;
			led_on_mode(6);
			FP_current_step = FP_STEP_DETETCT_FINGER;
			if (FP_time_for_rec){
				FP_current_step = FP_STEP_INPUT;
				send_command_to_FP(FP_CMD_GET_EMPTY_ID);
				FP_current_mode = FP_MODE_REC;
			}else{
				if (FP_try<4) FP_try ++;
				if (FP_try == 3) {
						alarm_flag[ALARM_FLAG_FP_TRY] = 250;
				}
			}
		}
/////////////          FINGER IN BASE
		if ((response == FP_RESPONSE_FINGER_IN_BASE)){
			led_on_mode(5);
			FP_detect_time = 10;
			FP_try = 0;
			alarm_flag[ALARM_FLAG_FP_TRY] = 0;
			if (FP_del_base){
				send_command_to_FP(FP_CMD_DEL_CHAR);
				FP_detect_time = 30;
			}
			FP_current_step = FP_STEP_DETETCT_FINGER;
		}
/////////////          STORE SUCC
		if ((response == FP_RESPONSE_STORE_SUCC)){
			FP_time_for_rec = 0;
			empty_id[0] = 0;
			empty_id[1] = 0;
			FP_current_mode = FP_MODE_MAIN;
			FP_current_step = FP_STEP_DETETCT_FINGER;
		}

		if ((response == FP_RESPONSE_FINGER_NOT_DETECT)){
			FP_current_step = FP_STEP_DETETCT_FINGER;
			FP_current_mode = FP_MODE_MAIN;
		}
		if (response == FP_RESPONSE_GET_EMPTY_ID){
			send_command_to_FP(FP_CMD_FINGER_DETECT);
		}
		if (response == FP_RESPONSE_DELETE_SUCC){
					FP_del_base = 0;
					FP_detect_time = 3;
					FP_current_step = FP_STEP_DETETCT_FINGER;
		}
		if (response == FP_RESPONSE_NO){
#ifdef DEBUG_FINGER
	send_string_to_GSM("ERROR RESPONSE$$$$$$$$$$$$$$$$$$\n\r");
#endif
		}
		return response;
	}else{
		if ((FP_current_step == FP_STEP_DETETCT_FINGER) & (!FP_detect_time)){
			led_off_mode(6);
			led_off_mode(5);
			FP_detect_time = 5;
			send_command_to_FP(FP_CMD_FINGER_DETECT);
//			FP_current_step = FP_STEP_WAIT;
		}
	}

	return 0;
}

int FP_parse_data(){
		switch(UART2_message[4]){
///////////////////////////////////////////////////////////////////////////////////           DETECT FINGER
		case FP_CMD_RESPONSE_DETECT_FINGER:
			if (UART2_message[8] == 0x00){
				if (UART2_message[10]){
					FP_current_step = FP_STEP_GET_IMAGE;
					send_command_to_FP(FP_CMD_GET_IMAGE);
#ifdef DEBUG_FINGER
	send_string_to_GSM("FINGER_DETECT!\n\r");
#endif
					return FP_RESPONSE_FINGER_DETECT;
				}else{
					return FP_RESPONSE_FINGER_NOT_DETECT;
				}
			}
		break;
///////////////////////////////////////////////////////////////////////////////////           GET IMAGE
		case FP_CMD_RESPONSE_GET_IMAGE:
			if ((UART2_message[6] == 0x02) && (UART2_message[8] == 0x00)){
#ifdef DEBUG_FINGER
	send_string_to_GSM("GET IMAGE SUCC!\n\r");
#endif
				if(FP_current_mode == FP_MODE_REC){
#ifdef DEBUG_FINGER
	send_string_to_GSM("GENERATE FOR ENROLL SEND!\n\r");
#endif
						send_command_to_FP(FP_CMD_GENERATE_FOR_ENROLL);
				}else{
					send_command_to_FP(FP_CMD_GENERATE);
				}
				FP_current_step = FP_STEP_GENERATE;
				return FP_RESPONSE_GET_IMAGE;
			}else{
				return FP_RESPONSE_FINGER_NOT_DETECT;
			}
		break;
///////////////////////////////////////////////////////////////////////////////////           GENERATE
		case FP_CMD_RESPONSE_GENERATE:
			if ((UART2_message[6] == 0x02) && (UART2_message[8] == 0x00)){
				if (FP_current_mode == FP_MODE_REC){
					if (FP_CMD_GENERATE_FOR_ENROLL[8] == 0x02){
						FP_CMD_GENERATE_FOR_ENROLL[8] = 0x00;
						FP_CMD_GENERATE_FOR_ENROLL[24] = 0x61;
						generate_for_enroll_step = 1;
						FP_current_step = FP_STEP_MERGE;
						send_command_to_FP(FP_CMD_MERGE);
					}else{
//						if (generate_for_enroll_step == FP_CMD_GENERATE_FOR_ENROLL[8]){
//
//							generate_for_enroll_step++;
//							send_command_to_FP(FP_CMD_FINGER_DETECT);
//						}else{
#ifdef DEBUG_FINGER
	send_string_to_GSM("GENERATE FOR ENROLL SUCC!\n\r");
#endif
							FP_CMD_GENERATE_FOR_ENROLL[8] = FP_CMD_GENERATE_FOR_ENROLL[8] + 1;
							FP_CMD_GENERATE_FOR_ENROLL[24] = FP_CMD_GENERATE_FOR_ENROLL[8] + 0x61;
							send_command_to_FP(FP_CMD_FINGER_DETECT);
//						}

					}
				}else{
#ifdef DEBUG_FINGER
	send_string_to_GSM("GENERATE SUCC!\n\r");
#endif
					FP_current_step = FP_STEP_SEARCH_IN_BASE;
					send_command_to_FP(FP_CMD_SEARCH);
				}
				return FP_RESPONSE_GENERATE;
			}else{
				FP_CMD_GENERATE_FOR_ENROLL[8] = 0x00;
				FP_CMD_GENERATE_FOR_ENROLL[24] = 0x61;
#ifdef DEBUG_FINGER
	send_string_to_GSM("GENERATE ERROR$$$$$$$$$$$$$$$\n\r");
#endif
				return FP_RESPONSE_FINGER_NOT_DETECT;
			}
		break;
///////////////////////////////////////////////////////////////////////////////////           MERGE
		case FP_CMD_RESPONSE_MERGE:
			if ((UART2_message[6] == 0x02) && (UART2_message[8] == 0x00)){
#ifdef DEBUG_FINGER
	send_string_to_GSM("MERGE SUCC!\n\r");
#endif
				FP_current_step = FP_STEP_STORE_CHAR;
				FP_CMD_STORE_CHAR[8] = empty_id[0];
				FP_CMD_STORE_CHAR[9] = empty_id[1];
				int sum = 0x55+0xAA+0x40+0x04 + empty_id[0] + empty_id[1];
				FP_CMD_STORE_CHAR[25] = (sum >> 8);
				FP_CMD_STORE_CHAR[24] = (sum & 0xFF);
				send_command_to_FP(FP_CMD_STORE_CHAR);

				return FP_RESPONSE_GET_IMAGE;
			}else{
#ifdef DEBUG_FINGER
	send_string_to_GSM("MERGE ERROR$$$$$$$$$$$$$$$$$$$\n\r");
#endif
				return FP_RESPONSE_FINGER_NOT_DETECT;
			}
		break;
///////////////////////////////////////////////////////////////////////////////////           SEARCH
		case FP_CMD_RESPONSE_SEARCH:
			if ((UART2_message[6] == 0x05) && (UART2_message[8] == 0x00)){
#ifdef DEBUG_FINGER
	send_string_to_GSM("####################FINGER IN BASE################!\n\r");
#endif
				return FP_RESPONSE_FINGER_IN_BASE;
			}else{
				if (UART2_message[8] == 0x11){
#ifdef DEBUG_FINGER
	send_string_to_GSM("FINGER NOT IN BASE!\n\r");
#endif
					return FP_RESPONSE_FINGER_NOT_IN_BASE;
				}
			}
		break;
///////////////////////////////////////////////////////////////////////////////////           GET_EMPTY_ID
		case FP_CMD_RESPONSE_GET_EMPTE_ID:
			if ((UART2_message[6] == 0x04)){
				empty_id[0] = UART2_message[10];
				empty_id[1] = UART2_message[11];
#ifdef DEBUG_FINGER
	send_string_to_GSM("GET EMPTY_ID!\n\r");
#endif
				return FP_RESPONSE_GET_EMPTY_ID;
			}else{
#ifdef DEBUG_FINGER
	send_string_to_GSM("GET EMPTY_ID ERROR$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\r");
#endif
				return FP_RESPONSE_FINGER_NOT_DETECT;
			}
		break;
///////////////////////////////////////////////////////////////////////////////////           STORE
		case FP_CMD_RESPONSE_STORE_CHAR:
			if ((UART2_message[6] == 0x02) && (UART2_message[8] == 0x00)){
#ifdef DEBUG_FINGER
	send_string_to_GSM("STORE SUCC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r");
#endif
				return FP_RESPONSE_STORE_SUCC;
			}else{
#ifdef DEBUG_FINGER
	send_string_to_GSM("STORE ERROR$$$$$$$$$$$$$\n\r");
#endif
				return FP_RESPONSE_FINGER_NOT_DETECT;
			}
		break;
///////////////////////////////////////////////////////////////////////////////////  			DELETE BASE
		case FP_CMD_RESPONSE_DELETE:
			if ((UART2_message[6] == 0x02) && (UART2_message[8] == 0x00)){
#ifdef DEBUG_FINGER
	send_string_to_GSM("DELETE OK!!!!");
#endif
				return FP_RESPONSE_DELETE_SUCC;
			}else{
#ifdef DEBUG_FINGER
	send_string_to_GSM("DELETE ERROR$$$$$$$$$$$$$\n\r");
#endif
				return FP_RESPONSE_FINGER_NOT_DETECT;
			}
		break;
///////////////////////////////////////////////////////////////////////////////////

		}
		FP_current_step = FP_STEP_DETETCT_FINGER;
	UART2_clear_message();
	return FP_RESPONSE_NO;
}

void send_command_to_FP(char * cmd){
	int i;
	for (i = 0;i<26;i++){
		send_char_to_UART2(cmd[i]);
	}
}



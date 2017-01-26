#include "input.h"
#include "guard_func.h"
#include "ADC_func.h"
#include "EEPROMfunc.h"
#include "led.h"
#include "my_string.h"

 uint8_t check_input_setting(int inp,int opt);

typedef struct INPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	uint8_t v_max;
	uint8_t v_min;
	uint8_t set_time_to_alarm;
	int16_t time_to_alarm;
	uint32_t adc_channel;
	uint8_t state;
	uint8_t alarm;
	char text[INPUT_TEXT_SIZE];

} INPUT_obj;

 INPUT_obj input[MAX_INPUT] ={
	    [0] = {	.port = INPUT_PORT, 	.pin = INPUT_1,			.mode = 0, .set_time_to_alarm = 1, .time_to_alarm = -1, .state = 0, .alarm = 0, .v_min = 3, .v_max = 7, .adc_channel = ADC_CHANNEL_1},
	    [1] = {	.port = INPUT_PORT, 	.pin = INPUT_2, 		.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0, .v_min = 3, .v_max = 7, .adc_channel = ADC_CHANNEL_4,},
	    [2] = {	.port = INPUT_PORT,		.pin = INPUT_3, 		.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0, .v_min = 3, .v_max = 7, .adc_channel = ADC_CHANNEL_5,},
	    [3] = {	.port = INPUT_PORT,		.pin = INPUT_4,			.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0, .v_min = 3, .v_max = 7, .adc_channel = ADC_CHANNEL_6,},
	    [4] = {	.port = INPUT_PORT, 	.pin = INPUT_5, 		.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0, .v_min = 3, .v_max = 7, .adc_channel = ADC_CHANNEL_7,},
	    [5] = {	.port = B_INPUT_PORT,  	.pin = B_INPUT_1, 		.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0},
	    [6] = {	.port = B_INPUT_PORT,  	.pin = B_INPUT_2,   	.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0},
	    [7] = {	.port = B_INPUT_PORT,  	.pin = B_INPUT_3,    	.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0},
	    [8] = {	.port = OPEN_CAP_PORT,	.pin = OPEN_CAP_PIN, 	.mode = 0, .set_time_to_alarm = 0, .time_to_alarm = -1, .state = 0, .alarm = 0, .text = "vzlom korpusa"},
 };

 uint8_t last_input_alarm = 0;
 int16_t time_to_alarm = -1;

void read_inputs_settings(){
 int i;
 int y;
 uint8_t temp;
 for (i = 0;i < MAX_INPUT;i++){
	 temp = EEPROMRead((EEPROM_input_v_max + i),1);
	 if (temp != 0xFE) input[i].v_max = temp;
	 temp = EEPROMRead((EEPROM_input_v_min + i),1);
	 if (temp != 0xFE) input[i].v_min = temp;
	 temp = EEPROMRead((EEPROM_input_mode + i),1);
	 if (temp != 0xFE) input[i].mode = temp;
	 temp = EEPROMRead((EEPROM_input_time_to_alarm + i),1);
	 if (temp != 0xFE) input[i].set_time_to_alarm = temp;
	 input[i].time_to_alarm = -1;
	 for (y = 0;y<INPUT_TEXT_SIZE;y++){
		 temp = EEPROMRead((EEPROM_input_text + y+(15*i)),1);
		 if (temp == 0xFE){
			 input[i].text[y] = 0;
			 break;
		 }else{
			 input[i].text[y] = temp;
		 }
	 }
 }
}


void set_input_settings(uint8_t inp, uint8_t v_min_t, uint8_t v_max_t,uint8_t mode_t,uint8_t time_to_alarm_t){
	inp--;
	if (inp < 0 || inp > MAX_INPUT) return;
	input[inp].v_max = v_max_t; EEPROMWrite((EEPROM_input_v_max + inp),input[inp].v_max,1);
	input[inp].v_min = v_min_t; EEPROMWrite((EEPROM_input_v_min + inp),input[inp].v_min,1);
	input[inp].mode = mode_t; EEPROMWrite((EEPROM_input_mode + inp),input[inp].mode,1);
	input[inp].set_time_to_alarm = time_to_alarm_t; EEPROMWrite((EEPROM_input_time_to_alarm + inp),input[inp].set_time_to_alarm,1);

#ifdef DEBUG
	send_string_to_UART3("Device: Set setting input! ID:");
	send_int_to_UART3((inp + 1));
	send_string_to_UART3(" MIN: ");
	send_int_to_UART3(input[inp].v_min);
	send_string_to_UART3(" MAX: ");
	send_int_to_UART3(input[inp].v_max);
	send_string_to_UART3("\n\rMODE_24H: ");
	check_input_setting(inp,INPUTS_MODE_24H) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("MODE_BUTTON_GUARD: ");
	check_input_setting(inp,INPUTS_MODE_BUTTON_GUARD) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("MODE_INVERS: ");
	check_input_setting(inp,INPUTS_MODE_INVERS) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3(" Time to alarm: ");
	send_int_to_UART3(input[inp].set_time_to_alarm);
	send_string_to_UART3(" \n\r ");
#endif
}

void set_input_text(uint8_t inp, char * text_t){
	inp--;
	if (inp < 0 || inp > MAX_INPUT) return;
	int i;
	for (i = 0;i < INPUT_TEXT_SIZE;i++){
		if (!text_t[5 + i]) {
			input[inp].text[i] = 0;
			EEPROMWrite((EEPROM_input_text + inp*INPUT_TEXT_SIZE + i),0xFE,1);
			break;
		}
		input[inp].text[i] = text_t[5 + i];
		EEPROMWrite((EEPROM_input_text + inp*INPUT_TEXT_SIZE + i),input[inp].text[i],1);
	}
#ifdef DEBUG
	send_string_to_UART3("Device: Set text input! ID:");
	send_int_to_UART3(inp);
	send_string_to_UART3(" TEXT: ");
	send_string_to_UART3(input[inp].text);
	send_string_to_UART3(" \n\r");
#endif
}

 int check_input(int input_t){
	 if (input_t<=5){
 		unsigned int adc_value;
 			adc_value = ADC_read(input[input_t - 1].adc_channel); // измерение со входа
#ifdef DEBUG_INPUTS_ADC
 			send_string_to_UART3("INPUT:");
 			send_int_to_UART3(input_t);
 			send_string_to_UART3(" Value:");
 			send_int_to_UART3(adc_value);
 			send_string_to_UART3(" Min:");
			send_int_to_UART3((input[input_t - 1].v_min)*300);
			send_string_to_UART3(" Max:");
			send_int_to_UART3(input[input_t - 1].v_max*300);
 			send_string_to_UART3(" \n\r");
#endif
 			if (((((input[input_t - 1].v_max*300) > adc_value) && ((input[input_t - 1].v_min)*300) < adc_value) ^ !(check_input_setting((input_t-1),INPUTS_MODE_INVERS)>0)) ){//вход не в норме
 				return 1;
 			}
 				return 0;
	 }else{
		 return !((GPIO_READ(input[input_t-1].port,input[input_t-1].pin) > 0) ^ (check_input_setting((input_t-1),INPUTS_MODE_INVERS) > 0));
	 }
  // 33 -> 0 V
  // 1350 -> 5.3 V
 // 2633 -> 10.3 V
 }


 void check_inputs(void){
 	int i;
 	for (i = 0;i<MAX_INPUT;i++){ // перебор входов
 		if (check_input(i+1)){
 			if (!input[i].state){
 #ifdef DEBUG_INPUTS
 	send_string_to_UART3("INPUT ");
 	send_int_to_UART3(i);
 	send_string_to_UART3(": ALERT!  \n\r");
 #endif
 				out_on_mode(i+1);
 				if (!input[i].state){
 						if (check_input_setting((i),INPUTS_MODE_BUTTON_GUARD)){ //если вход управл€ющий охраной
 							if (!get_guard_st()) guard_on_TM();
 							return;
 						}
 						if ((get_guard_st() || check_input_setting((i),INPUTS_MODE_24H))){ //если на охране или вход 24 часа
 								if (!input[i].alarm){ //если тревоги по этому входу небыло
 									last_input_alarm = i; //запомним последний сработавший вход
 	 								input[i].time_to_alarm = input[i].set_time_to_alarm * 5;
 	 								input[i].alarm = 1;
 							}
 						}
 				}
 				input[i].state = 1;
 			}
 		}else{ //вход в норме
 			if (input[i].state){
 				input[i].state = 0;
 				out_off_mode(i+1);
 				if (check_input_setting((i),INPUTS_MODE_BUTTON_GUARD)){//если вход управл€ющий охраной
 					if (get_guard_st()) guard_off();
 					return;
 				}
 #ifdef DEBUG_INPUTS
 	send_string_to_UART3("INPUT ");
 	send_int_to_UART3(i);
 	send_string_to_UART3(": NORMAL!  \n\r");
 #endif
 			}
 		}

 		if (!input[i].time_to_alarm){
 			input[i].time_to_alarm = -1;
 			alarm_on();
 		}
 	}
 }

 void check_time_to_alarm(){
	 int i;
	 for (i = 0;i< MAX_INPUT;i++){
 		if (input[i].time_to_alarm > 0) {
#ifdef DEBUG
 		 	send_string_to_UART3("INPUT #");
 		 	send_int_to_UART3(i);
 		 	send_string_to_UART3(" TIME TO ALARM : ");
 		 	send_int_to_UART3(input[i].time_to_alarm);
 		 	send_string_to_UART3("\n\r");
#endif
 		 	input[i].time_to_alarm--;
 		}
	 }
 }

 void clear_alarm_input(){
	 int i;
	 for (i = 0;i<MAX_INPUT;i++){
		 input[i].alarm = 0;
	 }
	 last_input_alarm = 0;
	 time_to_alarm = -1;
 }

 int get_alarm_input(){
	 return last_input_alarm;
 }

 void add_input_text(char * str, int inp){
	 int i;
	 int len_str = str_length(str);
	 for (i = 0; i < 70; i++){
		 if (!input[inp - 1].text[i]) break;
		 str[len_str + i] = input[inp - 1].text[i];
	 }
 }

 uint8_t check_input_setting(int inp,int opt){
	 return (input[inp].mode & opt);
 }

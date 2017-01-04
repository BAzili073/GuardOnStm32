#include "input.h"
#include "guard_func.h"
#include "ADC_func.h"
#include "EEPROMfunc.h"
#include "led.h"
#include "my_string.h"

typedef struct INPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	uint8_t v_max;
	uint8_t v_min;
	uint8_t time_to_alarm;
	uint32_t adc_channel;
	uint8_t state;
	char text[INPUT_TEXT_SIZE];

} INPUT_obj;

 INPUT_obj input[MAX_INPUT] ={
	    [0] = {	.port = INPUT_PORT, .pin = INPUT_1, .mode = 0, .v_min = 3, .v_max = 7, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_1, .state = 0},
	    [1] = {	.port = INPUT_PORT, .pin = INPUT_2, .mode = 0, .v_min = 3, .v_max = 7, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_4, .state = 0},
	    [2] = {	.port = INPUT_PORT, .pin = INPUT_3, .mode = 0, .v_min = 3, .v_max = 7, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_5, .state = 0},
	    [3] = {	.port = INPUT_PORT, .pin = INPUT_4, .mode = 0, .v_min = 3, .v_max = 7, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_6, .state = 0},
	    [4] = {	.port = INPUT_PORT, .pin = INPUT_5, .mode = 0, .v_min = 3, .v_max = 7, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_7, .state = 0},
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
	 if (temp != 0xFE) input[i].time_to_alarm = temp;
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


void set_input_settings(uint8_t inp, uint8_t v_max_t, uint8_t v_min_t,uint8_t mode_t,uint8_t time_to_alarm_t){
	inp--;
	input[inp].v_max = v_max_t; EEPROMWrite((EEPROM_input_v_max + inp),input[inp].v_max,1);
	input[inp].v_min = v_min_t; EEPROMWrite((EEPROM_input_v_min + inp),input[inp].v_min,1);
	input[inp].mode = mode_t; EEPROMWrite((EEPROM_input_mode + inp),input[inp].mode,1);
	input[inp].time_to_alarm = time_to_alarm_t; EEPROMWrite((EEPROM_input_time_to_alarm + inp),input[inp].time_to_alarm,1);

#ifdef DEBUG
	send_string_to_UART3("Device: Set setting input! ID:");
	send_int_to_UART3(inp);
	send_string_to_UART3(" MIN: ");
	send_int_to_UART3(v_min_t);
	send_string_to_UART3(" MAX: ");
	send_int_to_UART3(v_max_t);
	send_string_to_UART3(" Mode: ");
	send_int_to_UART3(mode_t);
	send_string_to_UART3(" Time to alarm: ");
	send_int_to_UART3(time_to_alarm_t);
	send_string_to_UART3(" \n\r ");
#endif
}

void set_input_text(uint8_t inp, char * text_t){
	inp--;
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
#endif
}

 int check_input(int input_t){
 		unsigned int adc_value;
 			adc_value = ADC_read(input[input_t - 1].adc_channel); // ��������� �� �����
 			if (((((input[input_t - 1].v_max*300) > adc_value) & ((input[input_t - 1].v_min)*300) < adc_value) ^ !((input[input_t - 1].mode & INPUTS_MODE_INVERS)>0)) ){//���� �� � �����
 				return 1;
 			}
 				return 0;
  // 33 -> 0 V
  // 1350 -> 5.3 V
 // 2633 -> 10.3 V
 }


 void check_inputs(void){
 	int i;
 	for (i = 1;i<=5;i++){ // ������� ������
 		if (check_input(i)){
 			if (!input[i-1].state){
 #ifdef DEBUG_INPUTS
 	send_string_to_UART3("INPUT ");
 	send_int_to_UART3(i);
 	send_string_to_UART3(": ALERT!  \n\r");
 #endif
 				input[i-1].state = 1;
 				out_on_mode(i);
 				if (last_input_alarm != i){
 						if (input[i-1].mode & INPUTS_MODE_BUTTON_GUARD){ //���� ���� ����������� �������
 							if (!get_guard_st()) guard_on();
 							return;
 						}
 						if ((get_guard_st() || (input[i-1].mode & INPUTS_MODE_24H)) & !get_alarm_st()){ //���� �� ������ ��� ���� 24 ����
 							last_input_alarm = i; //�������� ��������� ����������� ����
 							if ((time_to_alarm == -1) || ((input[i - 1].time_to_alarm * 5) < time_to_alarm)){ //���� ����� �� ������� ����
 								led_blink(OUT_MODE_GUARD,5,5);
 								time_to_alarm = input[i-1].time_to_alarm * 5;
 							}
 						}
 				}
 			}
 		}else{ //���� � �����
 			if (input[i-1].state){
 				input[i-1].state = 0;
 				out_off_mode(i);
 				if (input[i-1].mode & INPUTS_MODE_BUTTON_GUARD){//���� ���� ����������� �������
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
 	}

	if (!time_to_alarm){
		time_to_alarm = -1;
		alarm_on();
	}
 }

 void check_time_to_alarm(){
 		if (time_to_alarm > 0) time_to_alarm--;
 }

 void clear_alarm_input(){
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

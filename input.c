#include "input.h"
#include "guard_func.h"
#include "ADC_func.h"
#include "EEPROMfunc.h"
#include "led.h"

typedef struct INPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	uint8_t v_max;
	uint8_t v_min;
	uint8_t time_to_alarm;
	uint32_t adc_channel;
	uint8_t state;

} INPUT_obj;

 INPUT_obj input[MAX_INPUT] ={
	    [0] = {	.port = INPUT_PORT, .pin = INPUT_1, .mode = 0, .v_max = 7, .v_min = 3, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_1, .state = 0},
	    [1] = {	.port = INPUT_PORT, .pin = INPUT_2, .mode = 0, .v_max = 7, .v_min = 3, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_4, .state = 0},
	    [2] = {	.port = INPUT_PORT, .pin = INPUT_3, .mode = 0, .v_max = 7, .v_min = 3, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_5, .state = 0},
	    [3] = {	.port = INPUT_PORT, .pin = INPUT_4, .mode = 0, .v_max = 7, .v_min = 3, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_6, .state = 0},
	    [4] = {	.port = INPUT_PORT, .pin = INPUT_5, .mode = 0, .v_max = 7, .v_min = 3, .time_to_alarm = 0, .adc_channel = ADC_CHANNEL_7, .state = 0},
 };

 uint8_t last_input_alarm = 0;
 int16_t time_to_alarm = -1;

void read_inputs_settings(){
 int i;
 uint8_t temp;
 for (i = 0;i < MAX_INPUT;i++){
	 temp = EEPROMRead(EEPROM_input_v_max,1);
	 if (temp != 0xFE) input[i].v_max = temp;
	 temp = EEPROMRead(EEPROM_input_v_min,1);
	 if (temp != 0xFE) input[i].v_min = temp;
	 temp = EEPROMRead(EEPROM_input_mode,1);
	 if (temp != 0xFE) input[i].mode = temp;
	 temp = EEPROMRead(EEPROM_input_time_to_alarm,1);
	 if (temp != 0xFE) input[i].time_to_alarm = temp;
 }
 	 temp = EEPROMRead(EEPROM_time_alarm,1);
	 if (temp != 0xFE) time_to_alarm = temp;
}

void set_time_to_alarm(uint8_t time_to_alarm_t){
	time_to_alarm = time_to_alarm_t; EEPROMWrite(EEPROM_time_alarm,time_to_alarm_t,1);
}

void set_input_settings(uint8_t inp, uint8_t v_max_t, uint8_t v_min_t,uint8_t mode_t,uint8_t time_to_alarm_t){
	input[inp].v_max = v_max_t; EEPROMWrite((EEPROM_input_v_max + inp),input[inp].v_max,1);
	input[inp].v_min = v_min_t; EEPROMWrite((EEPROM_input_v_min + inp),input[inp].v_min,1);
	input[inp].mode = mode_t; EEPROMWrite((EEPROM_input_mode + inp),input[inp].mode,1);
	input[inp].time_to_alarm = time_to_alarm_t; EEPROMWrite((EEPROM_input_time_to_alarm + inp),input[inp].time_to_alarm,1);
}

 int check_input(int input_t){
 		unsigned int adc_value;
 			adc_value = ADC_read(input[input_t - 1].adc_channel); // измерение со входа
 			if (((((input[input_t - 1].v_max*300) > adc_value) & ((input[input_t - 1].v_min)*300) < adc_value) ^ !((input[input_t - 1].mode & INPUTS_MODE_INVERS)>0)) ){//вход не в норме
 				return 1;
 			}
 				return 0;
  // 33 -> 0 V
  // 1350 -> 5.3 V
 // 2633 -> 10.3 V
 }


 void check_inputs(void){
 	int i;
 	for (i = 1;i<=5;i++){ // перебор входов
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
 						if (input[i-1].mode & INPUTS_MODE_BUTTON_GUARD){ //если вход управл€ющий охраной
 							if (!get_guard_st()) guard_on();
 							return;
 						}
 						if ((get_guard_st() || (input[i-1].mode & INPUTS_MODE_24H)) & !get_alarm_st()){ //если на охране или вход 24 часа
 							last_input_alarm = i; //запомним последний сработавший вход
 							if ((time_to_alarm == -1) || ((input[i - 1].time_to_alarm * 5) < time_to_alarm)){ //если врем€ до тревоги нету
 								time_to_alarm = input[i-1].time_to_alarm * 5;
 								led_blink(OUT_MODE_GUARD,5,5);
 							}
 						}
 				}
 			}
 		}else{ //вход в норме
 			if (input[i-1].state){
 				input[i-1].state = 0;
 				out_off_mode(i);
 				if (input[i-1].mode & INPUTS_MODE_BUTTON_GUARD){//если вход управл€ющий охраной
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
 }

 void check_time_to_alarm(){
 		if (time_to_alarm > 0) time_to_alarm--;
 		if (!time_to_alarm){
 			time_to_alarm = -1;
 			alarm_on();
 		}
 }

 void clear_alarm_input(){
	 last_input_alarm = 0;
	 time_to_alarm = -1;
 }

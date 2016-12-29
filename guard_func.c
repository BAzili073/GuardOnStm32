#ifdef GUARD_H
#else
#define GUARD_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "modem.h"
#include "my_string.h"
#include "defines.h"
#include "EEPROMfunc.h"
#include "1-Wire.h"


unsigned int ADC_read(unsigned int chanel);

void output_on_mode(uint8_t mode);
void output_off_mode(uint8_t mode);
void output_on_hand(uint8_t output);
void output_off_hand(uint8_t output);
void led_blink_stop(uint8_t led_mode);
void led_blink(uint8_t led_mode, int8_t time_on,int8_t time_off);

void output_on(uint8_t output);
void output_off(uint8_t output);

int check_input(int input);
void guard_on();
void guard_off();
void guard_on_TM();
void check_inputs();
void check_battery();
void clear_last_control_guard();
void changed_guard_sms(int status);

uint8_t last_input_alarm = 0;

uint16_t u_battary;

int16_t time_to_alarm = -1;
uint16_t time_to_guard_on = -1;
uint16_t time_to_check_TM = 0;
int16_t time_set_to_guard_on = 0;
uint8_t device_settings = 0;
uint8_t alarm_st = 0;
uint8_t last_alarm = 0;
uint8_t guard_st = 0;


typedef struct OUTPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
} OUTPUT_obj;

OUTPUT_obj output[OUTPUT_MAX] = {
		[0] = {.port = OUTPUT_1_PORT, .pin = OUTPUT_1, .mode = OUT_MODE_LAMP,},
		[1] = {.port = OUTPUT_2_PORT, .pin = OUTPUT_2, .mode = OUT_MODE_GUARD,},
		[2] = {.port = OUTPUT_3_PORT, .pin = OUTPUT_3, .mode = OUT_MODE_TM,},
		[3] = {.port = OUTPUT_4_PORT, .pin = OUTPUT_4, .mode = OUT_MODE_LAMP,},
		[4] = {.port = OUTPUT_5_PORT, .pin = OUTPUT_5, .mode = OUT_MODE_LAMP,},
};

typedef struct LED_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	int8_t  blink_time;
	int8_t  blink_time_on;
	int8_t  blink_time_off;
} LED_obj;

LED_obj led[LED_MAX] = {
		[0] = { .port = LED_1_PORT, .pin = LED_1, .mode = OUT_MODE_LAMP, .blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[1] = { .port = LED_2_PORT, .pin = LED_2, .mode = OUT_MODE_GUARD, .blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[2] = { .port = LED_3_PORT, .pin = LED_3, .mode = OUT_MODE_TM, .blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[3] = { .port = LED_4_PORT, .pin = LED_4, .mode = OUT_MODE_GSM, .blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[4] = { .port = LED_5_PORT, .pin = LED_5, .mode = OUT_MODE_ALARM, .blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
};

 typedef struct INPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	uint16_t v_max;
	uint16_t v_min;
	uint16_t time_to_alarm;
	uint32_t adc_channel;
} INPUT_obj;

 INPUT_obj input[MAX_INPUT] ={
	    [0] = {	.port = INPUT_PORT, .pin = INPUT_1, .mode = 0, .v_max = 2000, .v_min = 1000, .adc_channel = ADC_CHANNEL_1},
	    [1] = {	.port = INPUT_PORT, .pin = INPUT_2, .mode = 0, .v_max = 2000, .v_min = 1000, .adc_channel = ADC_CHANNEL_4},
	    [2] = {	.port = INPUT_PORT, .pin = INPUT_3, .mode = 0, .v_max = 2000, .v_min = 1000, .adc_channel = ADC_CHANNEL_5},
	    [3] = {	.port = INPUT_PORT, .pin = INPUT_4, .mode = 0, .v_max = 2000, .v_min = 1000, .adc_channel = ADC_CHANNEL_6},
	    [4] = {	.port = INPUT_PORT, .pin = INPUT_5, .mode = 0, .v_max = 2000, .v_min = 1000, .adc_channel = ADC_CHANNEL_7},
 };

 typedef struct TEL_obj{
	char number[10];
	uint8_t  access;
} TEL_obj;

TEL_obj tel[MAX_TEL_NUMBERS] ={};

char tel_number_temp[10];

uint8_t last_control_ID_number = 254;
char last_control_guard[13];



/////////////////////////////////                      GUARD
void guard_on_TM(){
	if (time_set_to_guard_on){
		if (!time_to_guard_on){
			time_to_guard_on = time_set_to_guard_on;
		}
	}else{
		guard_on();
	}
}

void guard_on(){
		guard_st = GUARD_ON;
		out_on_mode(OUT_MODE_GUARD);
		changed_guard_sms(GUARD_ON);
}

void guard_off(){
	guard_st = GUARD_OFF;
	time_to_alarm = -1;
	out_off_mode(OUT_MODE_GUARD);
	alarm_off();
	changed_guard_sms(GUARD_OFF);
}

void changed_guard_sms(int status){
	str_add_str(output_sms_message,(status ? "na ohranu " : "snqt s ohranu " ));
	str_add_str(output_sms_message,last_control_guard);
	if (last_control_guard[0]) send_sms_message_for_all(output_sms_message,SMS_FUNCTION_CHANGE_GUARD_ALARM);
	clear_last_control_guard();
}

void alarm_on(){
	out_on_mode(OUT_MODE_ALARM);
	alarm_st = ALARM_ON;
}

void alarm_off(){
		out_off_mode(OUT_MODE_ALARM);
		alarm_st = ALARM_OFF;
}



/////////////////////////////////                      LEDS

void led_on(int id){
	GPIO_HIGH(led[id].port,(led[id].pin));
}

void led_off(int id){
	GPIO_LOW(led[id].port,(led[id].pin));
}

void led_on_mode(uint8_t mode){
	int i;
	for (i = 0;i<LED_MAX;i++){
		if (led[i].mode == mode) led_on(i);
	}
}

void led_off_mode(uint8_t mode){
	int i;
	for (i = 0;i<LED_MAX;i++){
		if (led[i].mode == mode) led_off(i);
	}
}

void led_blink(uint8_t mode, int8_t time_on,int8_t time_off){
	int i;
	for (i = 0;i < LED_MAX;i++){
		if (led[i].mode == mode){
			led[i].blink_time_on = time_on;
			led[i].blink_time_off = time_off;
		}
	}
}

void led_blink_stop(uint8_t mode){
	int i;
	for (i = 0;i < LED_MAX;i++){
		if (led[i].mode == mode){
			led[i].blink_time_on = LED_BLINK_STOP;
		}
	}
	led_off_mode(mode);
}

/////////////////////////////////                      OUTPUT
void output_on(uint8_t output_t){
	GPIO_HIGH(output[output_t-1].port,(output[output_t-1].pin));

}

void output_off(uint8_t output_t){
	GPIO_LOW((output[output_t-1].port),(output[output_t-1].pin));
}

void output_on_mode(uint8_t mode){
	for (int i = 1;i<=OUTPUT_MAX;i++){
		if (output[i-1].mode == mode) output_on(i);
	}
}

void output_off_mode(uint8_t mode){
	for (int i = 1;i<=OUTPUT_MAX;i++){
			if (output[i-1].mode == mode) output_off(i);
	}
}

void output_on_hand(uint8_t output_t){
	if (output[output_t].mode == OUT_MODE_HAND) output_on(output_t);
}

void output_off_hand(uint8_t output_t){
	if (output[output_t].mode == OUT_MODE_HAND) output_off(output_t);
}

void out_on_mode(uint8_t mode){
	led_on_mode(mode);
	output_on_mode(mode);
}
void out_off_mode(uint8_t mode){
	led_off_mode(mode);
	output_off_mode(mode);
}
///////////////////////////////////////////////////////////////////





void check_battery(){
	if (ADC_read(DET_220_CHANNEL) < u_battary); // измерение со входа
#ifdef DEBUG_220V
	send_string_to_UART3("DETECT 220v: ");
	send_int_to_UART3(ADC_read(DET_220_CHANNEL));
	send_string_to_UART3(" \n\r");
#endif
//	2115 - 11.2 V
//	2185 - 11.6 V
//	2271 - 12.0 V
//	2328 - 12.3 V
//	2396 - 12.6 V
//	2465 - 13.0 V

}

void check_inputs(void){
	int i;
	for (i = 1;i<=5;i++){ // перебор входов
		if (check_input(i)){
			led_on_mode(i);
			output_on_mode(i);
			if (last_alarm != i){
					if (input[i-1].mode & INPUTS_MODE_BUTTON_GUARD){ //если вход управл€ющий охраной
						if (!guard_st) guard_on();
						return;
					}
					if ((guard_st || (input[i-1].mode & INPUTS_MODE_24H)) & !alarm_st){ //если на охране или вход 24 часа
						last_alarm = i; //запомним последний сработавший вход
						if ((time_to_alarm == -1) || (input[i - 1].time_to_alarm < time_to_alarm)){ //если врем€ до тревоги нету
							time_to_alarm = input[i-1].time_to_alarm;
							last_input_alarm = i + 1;
						}
					}
			}
		}else{ //вход в норме
			led_off_mode(i);
			output_off_mode(i);
			if (input[i-1].mode & INPUTS_MODE_BUTTON_GUARD){//если вход управл€ющий охраной
				if (guard_st) guard_off();
				return;
			}

		}
	}
}

void check_TM(){
	out_off_mode(OUT_MODE_TM);
	if (!tm_key_number) return;
	int current_TM = one_wire_check_keys();
	if (current_TM != ONE_WIRE_KEY_DENY) {
		time_to_check_TM = 20;
		out_on_mode(OUT_MODE_TM);
		clear_last_control_guard();
		str_add_str(last_control_guard,"TM = ");
		str_add_num(last_control_guard,current_TM);
//		last_control_ID_number = current_TM + 100;
		if (guard_st){
			guard_off();
		}else{
			guard_on_TM();
		}
	}else{

	}
}

void read_settings(){
	uint8_t i = 0;
	uint8_t y = 0;

//////////////////   			 READ NUMBER
	for (i = 0;i< MAX_TEL_NUMBERS;i++){
		for (y = 0;y < 11; y++)
		tel[i].number[y] = EEPROMRead((EEPROM_tel_numbers + (i * 11) + y),1);
	}

//////////////////   			 READ TM ID
	tm_key_number = EEPROMRead(EEPROM_tms_numbers,1);
	for (i = 0;i< tm_key_number;i++){
		for (y = 0;y < 8; y++){
			TM_KEY[i].id[y] = EEPROMRead_id((EEPROM_tms_id + (i * 8) + y));
		}
	}


//////////////////   			 READ DS18b20 ID
	ds18x20_number = EEPROMRead(EEPROM_ds18x20_numbers,1);
	for (i = 0;i< ds18x20_number;i++){
		for (y = 0;y < 8; y++){
			DS18x20[i].id[y] = EEPROMRead_id((EEPROM_ds18x20_id + (i * 8) + y));
		}
	}

//////////////////

//////////////////

//////////////////

//////////////////

//////////////////


}


int check_input(int input_t){
		unsigned int adc_value;
			adc_value = ADC_read(input[input_t - 1].adc_channel); // измерение со входа
			if ((((input[input_t - 1].v_max > adc_value) & (adc_value > input[input_t - 1].v_min)) ^ !((input[input_t - 1].mode & INPUTS_MODE_INVERS)>0)) ){//вход не в норме
				return 1;
			}
				return 0;
 // 33 -> 0 V
 // 1350 -> 5.3 V
// 2633 -> 10.3 V
}

void clear_last_control_guard(){
	unsigned int i;
	for (i=0;i<13;i++) {
		last_control_guard[i] = 0;
	}
}


#endif


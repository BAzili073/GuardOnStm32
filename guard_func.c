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
int8_t  led_blink_time_on[8] = {-127,-127,-127,-127,-127,-127,-127,-127};
int8_t  led_blink_time_off[8] = {127,127,127,127,127,127,127,127};

int16_t time_to_alarm = -1;
uint16_t time_to_guard_on = -1;
int16_t time_set_to_guard_on = 0;
uint8_t alarm_st = 0;
uint8_t device_settings = 0;
uint8_t last_alarm = 0;
uint8_t guard_st = 0;
uint16_t outputs[5] = {OUTPUT_1,OUTPUT_2,OUTPUT_3,OUTPUT_4,OUTPUT_5};
uint8_t outputs_mode[5] = {0,1,2,3,4};
uint16_t leds[8] = {LED_1,LED_2,LED_3,LED_4,LED_5,LED_6,LED_7,LED_8};
uint8_t led_mode[8] = {1,2,3,4,5,6,7,7};

uint16_t inputs[5] = {INPUT_1,INPUT_2,INPUT_3,INPUT_4,INPUT_5};
uint16_t inputs_max[5] = {INPUT_1,INPUT_2,INPUT_3,INPUT_4,INPUT_5};
uint16_t inputs_min[5] = {INPUT_1,INPUT_2,INPUT_3,INPUT_4,INPUT_5};
uint8_t inputs_mode_bit[5] = {INPUT_MODE_NORMAL,INPUT_MODE_NORMAL,INPUT_MODE_NORMAL,INPUT_MODE_NORMAL,INPUT_MODE_NORMAL};
uint8_t inputs_time_to_alarm[5] = {0,0,0,0,0};

char tel_number[MAX_TEL_NUMBERS][10] = {};
uint8_t tel_access[MAX_TEL_NUMBERS] = {9,0,0,0,0};
char tel_number_temp[10];

uint8_t last_control_ID_number = 254;
char last_control_guard[13];
int temperature_time_check = 5;

void main_guard(){
	int current_TM = one_wire_check_keys();
	if (current_TM != ONE_WIRE_KEY_DENY) {
		clear_last_control_guard();
		str_add_str(last_control_guard,"TM = ");
		str_add_num(current_TM,last_control_guard);
//		last_control_ID_number = current_TM + 100;
		if (guard_st){
			guard_off();
		}else{
			guard_on_TM();
		}
	}
	check_inputs();
	check_battery();
	check_gsm_message();
}


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
		output_on_mode(OUTPUT_MODE_GUARD);
		changed_guard_sms(1);
}

void guard_off(){
	guard_st = GUARD_OFF;
	time_to_alarm = -1;
	output_off_mode(OUTPUT_MODE_GUARD);
	output_off_mode(OUTPUT_MODE_ALARM);
	changed_guard_sms(0);
}

void changed_guard_sms(int status){
	str_add_str(output_sms_message,(status ? "na ohranu " : "snqt s ohranu " ));
	str_add_str(output_sms_message,last_control_guard);
	if (last_control_guard[0]) send_sms_message_for_all(output_sms_message,SMS_FUNCTION_CHANGE_GUARD_ALARM);
	clear_last_control_guard();
}


void output_on_mode(uint8_t mode){
	for (int i = 1;i<=5;i++){
		if (outputs_mode[i-1] == mode) output_on(outputs[i-1]);
	}
}

void output_off_mode(uint8_t mode){
	for (int i = 1;i<=5;i++){
			if (outputs_mode[i-1] == mode) output_off(outputs[i-1]);
	}
}

void output_on_hand(uint8_t output){
	if (outputs_mode[output] == OUTPUT_MODE_HAND) output_on(output);
}

void output_off_hand(uint8_t output){
	if (outputs_mode[output] == OUTPUT_MODE_HAND) output_off(output);
}

void led_on_mode(uint8_t mode){
	int i;
	for (i = 1;i<=8;i++){
		if (led_mode[i-1] == mode) GPIO_HIGH(LED_PORT,(leds[i-1]));
	}
}

void led_off_mode(uint8_t mode){
	int i;
	for (i = 1;i<=8;i++){
		if (led_mode[i-1] == mode) GPIO_LOW(LED_PORT,(leds[i-1]));
	}
}

void output_on(uint8_t output){
	GPIO_HIGH(OUTPUT_PORT,(outputs[output-1]));
}

void output_off(uint8_t output){
	GPIO_LOW(OUTPUT_PORT,(outputs[output-1]));
}


void alarm_on(){
	output_on_mode(OUTPUT_MODE_ALARM);
	alarm_st = ALARM_ON;

}

void alarm_off(){
		output_off_mode(OUTPUT_MODE_ALARM);
		alarm_st = ALARM_OFF;
}

void led_blink(uint8_t led_mode, int8_t time_on,int8_t time_off){
	led_blink_time_on[led_mode - 1] = time_on;
	led_blink_time_off[led_mode - 1] = time_off;
}

void led_blink_stop(uint8_t mode){
	led_blink_time_on[mode - 1] = LED_BLINK_STOP;
	led_off_mode(mode);
}

void check_battery(){
	if (ADC_read(PIN_220) < u_battary); // ��������� �� �����
}

void check_inputs(void){
	int i;
	for (i = 1;i<=5;i++){ // ������� ������
		if (check_input(i)){
			led_on_mode(i);
			output_on_mode(OUTPUT_MODE_INPUTS + i);
			if (last_alarm != i){
					if (inputs_mode_bit[i-1] & INPUTS_MODE_BUTTON_GUARD){ //���� ���� ����������� �������
						if (!guard_st) guard_on();
						return;
					}
					if ((guard_st || (inputs_mode_bit[i-1] & INPUTS_MODE_24H)) & !alarm_st){ //���� �� ������ ��� ���� 24 ����
						last_alarm = i; //�������� ��������� ����������� ����
						if ((time_to_alarm == -1) || (inputs_time_to_alarm[i-1] < time_to_alarm)){ //���� ����� �� ������� ����
							time_to_alarm = inputs_time_to_alarm[i-1];
							last_input_alarm = i + 1;
						}
					}
			}
		}else{ //���� � �����
			led_off_mode(i);
			output_off_mode(OUTPUT_MODE_INPUTS + i);
			if (inputs_mode_bit[i-1] & INPUTS_MODE_BUTTON_GUARD){//���� ���� ����������� �������
				if (guard_st) guard_off();
				return;
			}

		}
	}
}

void read_settings(){
	uint8_t i = 0;
	uint8_t y = 0;


	for (i = 0;i< MAX_TEL_NUMBERS;i++){
		for (y = 0;y < 11; y++)
		tel_number[i][y] = EEPROMRead((EEPROM_tel_numbers + (i * 11) + y),1);
	}


	for (i = 0;i< MAX_TM;i++){
		for (y = 0;y < 8; y++)
		tm_id[i][y] = EEPROMRead((EEPROM_tms_id + (i * 8) + y),1);
	}


	for (i = 0;i< MAX_DS18x20;i++){
		for (y = 0;y < 8; y++)
		tm_id[i][y] = EEPROMRead((EEPROM_ds18x20_id + (i * 8) + y),1);
	}

}


int check_input(int input){
		unsigned int adc_value;
			adc_value = ADC_read(inputs[input - 1]); // ��������� �� �����
			if ((((inputs_max[input - 1] > adc_value) & (adc_value > inputs_min[input - 1])) ^ !((inputs_mode_bit[input-1] & INPUTS_MODE_INVERS)>0)) ){//���� �� � �����
				return 1;
			}else{
				return 0;
			}
}

void clear_last_control_guard(){
	unsigned int i;
	for (i=0;i<13;i++) {
		last_control_guard[i] = 0;
	}
}

#endif


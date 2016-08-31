#ifdef GUARD_H
#else
#define GUARD_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

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


uint16_t u_battary;
int8_t  led_blink_time_on[8] = {-127,-127,-127,-127,-127,-127,-127,-127};
int8_t  led_blink_time_off[8] = {127,127,127,127,127,127,127,127};

int16_t time_to_alarm = -1;
int16_t time_to_guard_on = 0;
int16_t time_set_to_guard_on = 0;
uint8_t alarm_st = 0;
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

uint8_t tel_number[MAX_TEL_NUMBERS][11];



void guard_on(){
	if (time_set_to_guard_on){
		time_to_guard_on = time_set_to_guard_on;

	}
	if(!time_to_guard_on){
		guard_st = GUARD_ON;
		output_on_mode(OUTPUT_MODE_GUARD);
	}
}
void guard_off(){
	guard_st = GUARD_OFF;
	time_to_guard_on = 0;
	time_to_alarm = -1;
	output_off_mode(OUTPUT_MODE_GUARD);
	output_off_mode(OUTPUT_MODE_ALARM);
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
	if (ADC_read(PIN_220) < u_battary); // измерение со входа
}

void check_inputs(void){
	int i;
	unsigned int adc_value;
	for (i = 1;i<=5;i++){ // перебор входов
		adc_value = ADC_read(inputs[i - 1]); // измерение со входа
		if ((((inputs_max[i - 1] > adc_value) & (adc_value > inputs_min[i - 1])) ^ !((inputs_mode_bit[i-1] & INPUTS_MODE_INVERS)>0)) ){ //вход не в норме
			led_on_mode(i);
			output_on_mode(OUTPUT_MODE_INPUTS + i);
			if (last_alarm != i){
					if (inputs_mode_bit[i-1] & INPUTS_MODE_BUTTON_GUARD){ //если вход управл€ющий охраной
						if (!guard_st) guard_on();
						return;
					}
					if ((guard_st || (inputs_mode_bit[i-1] & INPUTS_MODE_24H)) & !alarm_st){ //если на охране или вход 24 часа
						last_alarm = i; //запомним последний сработавший вход
						if ((time_to_alarm == -1) || (inputs_time_to_alarm[i-1] < time_to_alarm)){ //если врем€ до тревоги нету
							time_to_alarm = inputs_time_to_alarm[i-1];
						}
					}
			}
		}else{ //вход в норме
			led_off_mode(i);
			output_off_mode(OUTPUT_MODE_INPUTS + i);
			if (inputs_mode_bit[i-1] & INPUTS_MODE_BUTTON_GUARD){//если вход управл€ющий охраной
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

#endif


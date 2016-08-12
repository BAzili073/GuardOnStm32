#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"

unsigned int ADC_read(unsigned int chanel);

void output_on_mode(uint8_t mode);
void output_off_mode(uint8_t mode);
void output_on_hand(uint8_t output);
void output_off_hand(uint8_t output);
void led_blink_stop(uint8_t led_mode);
void led_blink(uint8_t led_mode, int8_t time_on,int8_t time_off);


uint16_t u_battary;
int8_t  led_blink_time_on[8] = {-127,-127,-127,-127,-127,-127,-127,-127};
int8_t  led_blink_time_off[8] = {127,127,127,127,127,127,127,127};

int16_t time_to_alarm = -1;
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


void guard_on(){
	guard_st = GUARD_ON;
	output_on_mode(OUTPUT_MODE_GUARD);
}
void guard_off(){
	guard_st = GUARD_OFF;
	output_off_mode(OUTPUT_MODE_GUARD);
}

void output_on_mode(uint8_t mode){
	for (int i = 1;i<=5;i++){
		if (outputs_mode[i-1] == mode) GPIO_HIGH(OUTPUT_PORT,(outputs[i-1]));
	}
}

void output_off_mode(uint8_t mode){
	for (int i = 1;i<=5;i++){
			if (outputs_mode[i-1] == mode) GPIO_LOW(OUTPUT_PORT,(outputs[i-1]));
	}
}

void output_on_hand(uint8_t output){
	if (outputs_mode[output] == OUTPUT_MODE_HAND) GPIO_HIGH(OUTPUT_PORT,(outputs[output-1]));
}

void output_off_hand(uint8_t output){
	if (outputs_mode[output] == OUTPUT_MODE_HAND) GPIO_LOW(OUTPUT_PORT,(outputs[output-1]));
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
	unsigned int adc_value;
	for (i = 1;i<=5;i++){ // ������� ������
		adc_value = ADC_read(inputs[i - 1]); // ��������� �� �����
		if ((((inputs_max[i - 1] > adc_value) & (adc_value > inputs_min[i - 1])) ^ !((inputs_mode_bit[i-1] & INPUTS_MODE_INVERS)>0)) ){ //���� �� � �����
			led_on_mode(i);
			output_on_mode(OUTPUT_MODE_INPUTS + i);
			if (last_alarm != i){
					if (inputs_mode_bit[i-1] & INPUTS_MODE_BUTTON_GUARD){ //���� ���� ����������� �������
						if (!guard_st) guard_on();
						return;
					}
					if (guard_st || (inputs_mode_bit[i-1] & INPUTS_MODE_24H) ){ //���� �� ������ ��� ���� 24 ����
						last_alarm = i; //�������� ��������� ����������� ����
						if ((time_to_alarm == -1) || (inputs_time_to_alarm[i-1] < time_to_alarm)){ //���� ����� �� ������� ����
							time_to_alarm = inputs_time_to_alarm[i-1];
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



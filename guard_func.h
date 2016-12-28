#ifdef GUARD_H
#else
#define GUARD_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
uint8_t device_settings;
int16_t time_to_alarm;
int16_t time_to_guard_on;
uint8_t alarm_st;
uint8_t guard_st;
uint8_t last_alarm;

typedef struct LED_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	int8_t  blink_time_on;
	int8_t  blink_time_off;
} LED_obj;

LED_obj led[LED_MAX];

typedef struct OUTPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
} OUTPUT_obj;

OUTPUT_obj output[OUTPUT_MAX];

 GPIO_TypeDef * outputs_port[5];




int temperature_time_check;




void guard_on();
void guard_off();

void alarm_on();
void alarm_off();

void output_on_mode(uint8_t mode);
void output_off_mode(uint8_t mode);
void output_on_hand(uint8_t output);
void output_off_hand(uint8_t output);

void led_on_mode(uint8_t led);
void led_off_mode(uint8_t led);
void led_blink_stop(uint8_t led_mode);
void led_blink(uint8_t led_mode, int8_t time_on,int8_t time_off);
void led_on(int id);
void led_off(int id);

void check_inputs();
int check_input(int input);
void check_TM();


char tel_number[MAX_TEL_NUMBERS][10];
uint8_t tel_access[MAX_TEL_NUMBERS];

uint8_t last_control_ID_number;
char last_control_guard[13];

char tel_number_temp[10];

#endif

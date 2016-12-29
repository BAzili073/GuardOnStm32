#ifdef GUARD_H
#else
#define GUARD_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
uint8_t device_settings;
int16_t time_to_guard_on;
uint8_t alarm_st;
uint8_t guard_st;


typedef struct OUTPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
} OUTPUT_obj;

OUTPUT_obj output[OUTPUT_MAX];

 GPIO_TypeDef * outputs_port[5];





void out_on_mode();
void out_off_mode();

void guard_on();
void guard_off();

void alarm_on();
void alarm_off();

uint8_t get_guard_st();
uint8_t get_alarm_st();

void check_TM();


char tel_number[MAX_TEL_NUMBERS][10];
uint8_t tel_access[MAX_TEL_NUMBERS];
uint16_t time_to_check_TM;
uint8_t last_control_ID_number;
char last_control_guard[13];

char tel_number_temp[10];

#endif

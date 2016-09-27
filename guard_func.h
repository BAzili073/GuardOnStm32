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
uint16_t outputs[5];
uint8_t outputs_mode[5];
uint16_t leds[8];
uint8_t leds_mode[8];
uint16_t inputs[5];
uint16_t inputs_max[5];
uint16_t inputs_min[5];
uint16_t inputs_mode[5];
uint8_t inputs_time_to_alarm[5];
int8_t  led_blink_time_on[8];
int8_t  led_blink_time_off[8];

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

void check_inputs();


uint8_t tel_number[MAX_TEL_NUMBERS][10];
uint8_t tel_access[MAX_TEL_NUMBERS];
uint8_t last_control_ID_number;


#endif

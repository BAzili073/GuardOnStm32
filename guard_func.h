#ifdef GUARD_H
#else
#define GUARD_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "defines.h"


void out_on_mode(uint8_t mode);
void out_off_mode(uint8_t mode);
void check_time_to_guard_on();
void check_lamp_blink();
void read_guard_settings();
void guard_on();
void guard_off();

void alarm_on();
void alarm_off();
void set_device_setting(uint8_t settings, uint8_t time_to_guard_t);


uint8_t get_guard_st();
uint8_t get_alarm_st();

void TM_check_time();

uint8_t last_control_ID_number;
char last_control_guard[13];


char tel_number_temp[10];

#endif

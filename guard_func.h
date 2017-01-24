#ifdef GUARD_H
#else
#define GUARD_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "defines.h"


void out_on_mode(uint8_t mode);
void out_off_mode(uint8_t mode);
void check_time_to_guard_on();

#define DEVICE_SETTING_SMS_AT_STARTUP 1<<7
#define DEVICE_SETTING_SMS_AT_CHANGE_GUARD 1<<6
#define DEVICE_SETTING_SMS_AT_SMS_COMMAND 1<<5
#define DEVICE_SETTING_BAN_OUTPUT_CALL 1<<4
#define DEVICE_SETTING_CHANGE_GUARD_USE_CALL 1<<3
#define DEVICE_SETTING_SMS_AT_ALARM 1<<2
#define DEVICE_SETTING_AUTO_GUARD_AT_START 1<<1
#define DEVICE_SETTING_SMS_AT_UNCORRECT_SMS 1<<0

void check_lamp_blink();
void read_device_settings();
void guard_on();
void guard_off();
void check_guard_change();
void alarm_on();
void alarm_off();
void set_device_setting(uint8_t settings, uint8_t time_to_guard_t, uint8_t time_alarm_t);
void set_new_guard_st(uint8_t new_st);
uint8_t check_device_setting(uint8_t opt);
void check_TM();
void check_time_to_reset();
void set_time_to_reset(uint8_t day);

uint8_t get_guard_st();
uint8_t get_alarm_st();
uint8_t get_powered();
void TM_check_time();
void check_battery();
void check_lamp_blink_time();

uint8_t last_control_ID_number;
char last_control_guard[13];


#endif

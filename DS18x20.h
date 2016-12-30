#ifdef DS18x20_H
#else
#define DS18x20_H


#include "defines.h"
#include "EEPROMfunc.h"

void read_ds18x20_settings();
void add_DS18x20(uint8_t id[8]);
int find_ds18x20(uint8_t id[8]);
void get_all_temp();
void check_temperature();
void check_temp();
uint8_t get_flag_conv();

#define DS18X20_ALARM_NORM 0
#define DS18X20_ALARM_UP 1
#define DS18X20_ALARM_DOWN 2

#define DS18X20_SETTINGS_SMS 			0b00000001
#define DS18X20_SETTINGS_CONTROL_OUT 	0b00000010
#define DS18X20_SETTINGS_CONTROL_INVER 	0b00000100


typedef struct DS18x20_obj{
	uint8_t id[8];
	int16_t  last_temp;
	uint16_t max_temp;
	uint16_t min_temp;
	uint8_t alarm;
	uint8_t settings;
} DS18x20_obj;


#endif

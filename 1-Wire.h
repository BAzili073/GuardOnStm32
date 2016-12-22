#include "defines.h"

#define DS18X20_ALARM_NORM 0
#define DS18X20_ALARM_UP 1
#define DS18X20_ALARM_DOWN 2

#define DS18X20_SETTINGS_SMS 			0b00000001
#define DS18X20_SETTINGS_CONTROL_OUT 	0b00000010
#define DS18X20_SETTINGS_CONTROL_INVER 	0b00000100


void one_wire_write_bit(uint8_t bit);
uint8_t one_wire_read_bit();
void one_wire_write_byte(uint8_t data);
char one_wire_send_presence();
uint8_t one_wire_read_bit();
void one_wire_write_byte(uint8_t data);
void one_wire_write_bit(uint8_t bit);
uint8_t one_wire_read_rom(uint8_t * buf);
uint8_t * one_wire_enum_next();
int16_t one_wire_read_temp_to_address(uint8_t address[8]);
uint8_t one_wire_start_conversion_temp();
uint8_t one_wire_check_keys();
void get_all_temp();

uint8_t ds18x20_id[MAX_DS18x20][8];
char tm_id[MAX_TM][8];

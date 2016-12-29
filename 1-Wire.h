#ifdef ONE_WIRE_H
#else
#define ONE_WIRE_H

#include "defines.h"


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




typedef struct TM_KEY_obj{
	uint8_t id[8];
} TM_KEY_obj ;

TM_KEY_obj TM_KEY[MAX_TM];
uint8_t ds18x20_number;
uint8_t tm_key_number;

int8_t time_to_check_temp;


#endif

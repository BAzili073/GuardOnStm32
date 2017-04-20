#ifdef TM_KEY_H
#else
#define TM_KEY_H
#include "defines.h"
#include INCLUDED_FILE_1
#include INCLUDED_FILE_2
#include "EEPROMfunc.h"
#include "led.h"

void read_TM_KEY_settings();
void add_TM_key(uint8_t id[8]);
uint8_t get_tm_key_number();

typedef struct TM_KEY_obj{
	uint8_t id[8];
	char name[10];
} TM_KEY_obj ;

uint8_t find_key(uint8_t key[8]);







#endif

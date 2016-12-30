#include "TM_KEY.h"
TM_KEY_obj TM_KEY[MAX_TM];

uint8_t tm_key_number;

uint8_t get_tm_key_number(){
	return tm_key_number;
}

void read_TM_KEY_settings(){
	int i,y;
	uint8_t temp;
	temp = EEPROMRead(EEPROM_tms_numbers,1);
	if (temp != 0xFE )tm_key_number = temp;
	for (i = 0;i< tm_key_number;i++){
		for (y = 0;y < 8; y++){
			TM_KEY[i].id[y] = EEPROMRead_id((EEPROM_tms_id + (i * 8) + y));
		}
		for (y = 0;y < 10; y++){
			TM_KEY[i].name[y] = EEPROMRead_id((EEPROM_tms_name + (i * 10) + y));
			if (TM_KEY[i].name[y] == 0xFE) break;
		}
	}
}

//void set_TM_KEY_settings(uint8_t tm, uint8_t name_tm){
//
//}

void add_TM_key(uint8_t id[8]){
	if (tm_key_number < MAX_TM){
		led_on(4);
		tm_key_number++;
		EEPROMWrite(EEPROM_tms_numbers,tm_key_number,1);
		int i;
		for (i = 0;i<8;i++){
			TM_KEY[tm_key_number-1].id[i] = id[i];
			EEPROMWrite((EEPROM_tms_id + ((tm_key_number-1)*8) + i),id[i],1);
		}
	}
}

uint8_t find_key(uint8_t key[8]){
	uint8_t i;
	uint8_t y;
	uint8_t ok;
	for (i = 0; i < MAX_TM;i++){
		ok = 1;
		for (y = 0;y<8;y++){
			if (key[y] != TM_KEY[i].id[y]){
				ok = 0;
				break;
			}
		}
		if (ok) return i;
//		if (key == keys[i]) return 1;
	}
	return ONE_WIRE_KEY_DENY;
}

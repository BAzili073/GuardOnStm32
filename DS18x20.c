#include "DS18x20.h"
#include "modem_module.h"


DS18x20_obj DS18x20[MAX_DS18x20] = {
		[0] = {.max_temp =  25, .min_temp = -20},
//		[1] = {.id = {0x28,0x62,0x57,0xA0,0x04,0x00,0x00,0x40}},
};

uint8_t ds18x20_number = 0;
uint8_t time_to_check_temp = 0;
uint8_t flag_conv = 0;

void read_ds18x20_settings(){
	int i;
	int y;
	uint8_t temp;
	temp = EEPROMRead(EEPROM_ds18x20_numbers,1);
	if (temp != 0xFE ) ds18x20_number = temp;
	for (i = 0;i< ds18x20_number;i++){
		for (y = 0;y < 8; y++){
			DS18x20[i].id[y] = EEPROMRead_id((EEPROM_ds18x20_id + (i * 8) + y));
		}
		temp = EEPROMRead ((EEPROM_ds18x20_min + i),1);
		if (temp != 0xFE ) DS18x20[i].min_temp = temp;
		temp = EEPROMRead ((EEPROM_ds18x20_max + i),1);
		if (temp != 0xFE )  DS18x20[i].max_temp = temp;
		temp = EEPROMRead ((EEPROM_ds18x20_settings + i),1);
		if (temp != 0xFE )  DS18x20[i].settings = temp;
	}
}

void set_ds18x20_settings(uint8_t ds, uint8_t min_temp_t, uint8_t max_temp_t ,uint8_t settings_t){
	DS18x20[ds].min_temp = min_temp_t; EEPROMWrite((EEPROM_ds18x20_min + ds),DS18x20[ds].min_temp,1);
	DS18x20[ds].max_temp = max_temp_t; EEPROMWrite((EEPROM_ds18x20_max + ds),DS18x20[ds].max_temp,1);
	DS18x20[ds].settings = settings_t; EEPROMWrite((EEPROM_ds18x20_settings + ds),DS18x20[ds].settings,1);
}


void time_check_temp(){
	if (time_to_check_temp > 0) time_to_check_temp--;
}
void add_DS18x20(uint8_t id[8]){
	if (ds18x20_number < MAX_DS18x20){
		led_on(4);
		ds18x20_number++;
		EEPROMWrite(EEPROM_ds18x20_numbers,ds18x20_number,1);
		int i;
		for (i = 0;i<8;i++){
			DS18x20[ds18x20_number-1].id[i] = id[i];
			EEPROMWrite((EEPROM_ds18x20_id + ((ds18x20_number-1)*8) + i),id[i],1);
		}
	}
}

int find_ds18x20(uint8_t id[8]){
	uint8_t i;
	uint8_t y;
	uint8_t ok;
	for (i = 0; i < MAX_DS18x20;i++){
		ok = 1;
		for (y = 0;y<8;y++){
			if (id[y] != DS18x20[i].id[y]){
				ok = 0;
				break;
			}
		}
		if (ok) return 1;
//		if (key == keys[i]) return 1;
	}
	return 0;
}

void get_all_temp(){
	int i;
	for (i = 0;i<ds18x20_number;i++){
		DS18x20[i].last_temp = one_wire_read_temp_to_address(DS18x20[i].id);
	}
}

uint8_t get_flag_conv(){
	return flag_conv;
}
void check_temperature(){
	if (!ds18x20_number) return;
	if (!time_to_check_temp && !flag_conv) {
		one_wire_start_conversion_temp();
		flag_conv = 1;
		time_to_check_temp = 8;
		return;
	}else if (time_to_check_temp != 0){
		return;
	}
	time_to_check_temp = TIME_CHECK_DS18B20;
	flag_conv = 0;
	int i;
	int y;
	for (i = 0;i<MAX_DS18x20;i++){
			if ((DS18x20[i].id[0] == 0x28) || (DS18x20[i].id[0] == 0x22) || (DS18x20[i].id[0] == 0x10)) {
				DS18x20[i].last_temp = one_wire_read_temp_to_address (DS18x20[i].id);
#ifdef DEBUG_DS18x20
	send_string_to_UART3("Adress: ");
	int k;
    for (uint8_t k = 0; k < 8; k++) {
    	char d = DS18x20[i].id[k];
		send_char_to_UART3((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
		send_char_to_UART3((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
		send_char_to_UART3(' ');
    }
    send_string_to_UART3("Temp: ");
    if (DS18x20[i].last_temp == ONE_WIRE_CONVERSION_ERROR){
    		send_string_to_UART3("Error read temp!  ");
    }else{
    	send_int_to_UART3((DS18x20[i].last_temp));

    }
	send_string_to_UART3(" \n\r");

#endif
	if (DS18x20[i].last_temp != ONE_WIRE_CONVERSION_ERROR){
				if (((DS18x20[i].last_temp) > DS18x20[i].max_temp)){
					if (DS18x20[i].alarm == DS18X20_ALARM_NORM){
						if (DS18x20[i].settings & DS18X20_SETTINGS_CONTROL_OUT){
							for (y = 3;y<8;y++){
								if (DS18x20[i].settings & (1<<y)){
									if (DS18x20[i].settings & DS18X20_SETTINGS_CONTROL_INVER){
										output_off_hand(y-3);
									}else{
										output_on_hand(y-3);
									}
								}
							}
						}
						if (DS18x20[i].settings && DS18X20_SETTINGS_SMS){
							str_add_str(output_sms_message,"temperatura datCika #");
											str_add_num(i,output_sms_message);
											str_add_str(output_sms_message,"vQwe normQ: ");
											str_add_num(DS18x20[i].last_temp,output_sms_message);
#ifdef DEBUG_DS18x20
		send_string_to_UART3("output_sms_message");
		send_string_to_UART3(" \n\r");
#endif
											send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
						}

						DS18x20[i].alarm = DS18X20_ALARM_UP;
					}
				}else if (((DS18x20[i].last_temp) < DS18x20[i].min_temp)){
						if (DS18x20[i].alarm == DS18X20_ALARM_NORM){
							if (DS18x20[i].settings & DS18X20_SETTINGS_CONTROL_OUT){
								for (y = 3;y<8;y++){
									if (DS18x20[i].settings && (1<<y)){
										if (DS18x20[i].settings & DS18X20_SETTINGS_CONTROL_INVER){
											output_off_hand(y-3);
										}else{
											output_on_hand(y-3);
										}
									}
								}
							}
							if (DS18x20[i].settings && DS18X20_SETTINGS_SMS){
								str_add_str(output_sms_message,"temperatura datCika #");
								str_add_num(i,output_sms_message);
								str_add_str(output_sms_message," nije normQ: ");
								str_add_num(DS18x20[i].last_temp,output_sms_message);
		#ifdef DEBUG_DS18x20
				send_string_to_UART3("output_sms_message");
				send_string_to_UART3(" \n\r");
		#endif
								send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
							}
							DS18x20[i].alarm = DS18X20_ALARM_DOWN;
						}
				}else{
					if (DS18x20[i].alarm != DS18X20_ALARM_NORM){
						if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_OUT){
							for (y = 3;y<8;y++){
								if (DS18x20[i].settings && (1<<y)){
									if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_INVER){
										output_on_hand(y-3);
									}else{
										output_off_hand(y-3);
									}
								}
							}
						}
						if (DS18x20[i].settings && DS18X20_SETTINGS_SMS){
							str_add_str(output_sms_message,"temperatura datCika #");
							str_add_num(i,output_sms_message);
							str_add_str(output_sms_message," v norme: ");
							str_add_num(DS18x20[i].last_temp,output_sms_message);
		#ifdef DEBUG_DS18x20
				send_string_to_UART3("output_sms_message");
				send_string_to_UART3(" \n\r");
		#endif
							send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
						}
					DS18x20[i].alarm = DS18X20_ALARM_NORM;
					}

				}
			}
			}
		}
}

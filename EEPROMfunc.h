#ifdef EEPROM_H
#else
#define EEPROM_H

#define start_EEPROM 0x08080000
#define EEPROM_device_settings		(0)
#define EEPROM_time_set_alarm		(EEPROM_device_settings + 1)
#define EEPROM_time_to_guard		(EEPROM_time_set_alarm + 1)
#define EEPROM_tel_numbers 			(EEPROM_time_to_guard + 1)
#define EEPROM_tel_access 			(EEPROM_tel_numbers + (10*MAX_TEL_NUMBERS))
#define EEPROM_tms_id 				(EEPROM_tel_access + MAX_TEL_NUMBERS)
#define EEPROM_tms_name 			(EEPROM_tms_id + (8 * MAX_TM))
#define EEPROM_ds18x20_id 			(EEPROM_tms_name + (10 * MAX_TM))
#define EEPROM_ds18x20_numbers 		(EEPROM_ds18x20_id + (8 * MAX_DS18x20))
#define EEPROM_tms_numbers 			(EEPROM_ds18x20_numbers + 1)
#define EEPROM_ds18x20_min			(EEPROM_tms_numbers + 1)
#define EEPROM_ds18x20_max			(EEPROM_ds18x20_min + MAX_DS18x20)
#define EEPROM_ds18x20_settings		(EEPROM_ds18x20_max + MAX_DS18x20)
#define EEPROM_input_v_min			(EEPROM_ds18x20_settings + MAX_DS18x20)
#define EEPROM_input_v_max			(EEPROM_input_v_min + MAX_INPUT)
#define EEPROM_input_mode			(EEPROM_input_v_max + MAX_INPUT)
#define EEPROM_input_time_to_alarm	(EEPROM_input_mode + MAX_INPUT)
#define EEPROM_input_text 			(EEPROM_input_time_to_alarm + MAX_INPUT)
#define EEPROM_output_mode			(EEPROM_input_text + MAX_INPUT * INPUT_TEXT_SIZE)
#define EEPROM_led_mode				(EEPROM_output_mode + MAX_OUTPUT)
#define EEPROM_time_to_reset		(EEPROM_led_mode + MAX_LED)
#define EEPROM_time_to_report		(EEPROM_time_to_reset + 1)
#define EEPROM_POWER_220V			(EEPROM_time_to_report + 1)
#define EEPROM_POWER_BATT			(EEPROM_POWER_220V + 2)
//#define EEPROM_		(EEPROM_POWER_COEF + 1)

uint32_t *EEPROMAddress;

void EEPROMWrite(int address,uint32_t data,char bytes);
uint32_t EEPROMRead(uint32_t address, char bytes);
uint8_t EEPROMRead_id(uint32_t address);


#endif

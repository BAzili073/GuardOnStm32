#include "guard_func.h"
#include "modem_module.h"
#include "my_string.h"
#include "EEPROMfunc.h"
#include "1-Wire.h"
#include "UART.h"
#include "led.h"
#include "ADC_func.h"
#include "output.h"
#include "input.h"
#include "TM_KEY.h"
#include "DS18x20.h"
#include "modem.h"

 //U [volt] * 10

extern TEL_obj tel[MAX_TEL_NUMBERS];
int checkValidCode(int Step);
void guard_on_TM();

void clear_last_control_guard();
void changed_guard_sms(int status);
uint8_t new_guard_st;

uint8_t u_220V = 140;
uint8_t u_batt = 130;
uint8_t u_coef = 100;
uint8_t powered = POWERED_220V;
uint8_t device_settings = 0b00000000;
uint8_t time_set_alarm = 6;

int16_t time_to_guard_on = -1;
uint16_t time_to_check_TM = 0;
int16_t time_set_to_guard_on = 0;

uint8_t alarm_st = 0;
uint8_t guard_st = 0;
int8_t lamp_blink_time = 5;

char tel_number_temp[10];

uint8_t last_control_ID_number = 254;
char last_control_guard[13];

uint8_t time_to_full_reset = 0;
int16_t time_to_report = -1;
uint8_t set_time_to_report = 254;

void check_time_to_reset(){
	if (time_to_full_reset) {
		time_to_full_reset --;
		if (!time_to_full_reset) FULL_RESET();
	}
}

void check_time_to_report(){
	if (time_to_report) {
		time_to_report --;
	}
}
void check_send_report(){
	if (!time_to_report){
		sms_command_r();
		time_to_report = set_time_to_report;
	}
}
void set_time_to_reset(uint8_t day){
	time_to_full_reset = day;
	if (!day) day = 0xFE;
	EEPROMWrite(EEPROM_time_to_reset,day,1);
	send_string_to_UART3("Device: Time to full reset set each: ");
	send_int_to_UART3(day);
	send_string_to_UART3(" days");
}

void setting_time_to_report(uint8_t hour){
	set_time_to_report = hour;
	EEPROMWrite(EEPROM_time_to_report,hour,1);
	if (!hour){
		time_to_report = -1;
		EEPROMWrite(EEPROM_time_to_report,0xFE,1);
	}else{
		time_to_report = set_time_to_report;
	}
#ifdef DEBUG
	send_string_to_UART3("Device: Time to report set each: ");
	send_int_to_UART3(hour);
	send_string_to_UART3(" hours");
#endif
}

void read_device_settings(){
	 uint8_t temp;
	 temp = EEPROMRead(EEPROM_time_to_guard,1);
	 if (temp != 0xFE) time_set_to_guard_on = temp;
	 temp = EEPROMRead(EEPROM_device_settings,1);
	 if (temp != 0xFE) device_settings = temp;
	 temp = EEPROMRead(EEPROM_time_set_alarm,1);
	 if (temp != 0xFE) time_set_alarm = temp;
	 temp = EEPROMRead((EEPROM_time_to_reset),1);
	 if (temp != 0xFE) time_to_full_reset = temp;
	 temp = EEPROMRead((EEPROM_time_to_report),1);
	 if (temp != 0xFE) set_time_to_report = temp;
	 if (set_time_to_report != 0xFE && set_time_to_report) time_to_report = set_time_to_report;
	 temp = EEPROMRead((EEPROM_POWER_220V),1);
	 if (temp != 0xFE) u_220V = temp;
	 else setting_powered(1);

	 temp = EEPROMRead((EEPROM_POWER_BATT),1);
	 if (temp != 0xFE) u_batt = temp;
	 else u_batt = u_220V - 11;
}

void set_device_setting(uint8_t settings, uint8_t time_to_guard_t, uint8_t time_alarm_t){
	device_settings = settings; EEPROMWrite(EEPROM_device_settings,settings,1);
	time_to_guard_on = time_to_guard_t; EEPROMWrite(EEPROM_time_to_guard,time_to_guard_t,1);
	time_set_alarm = time_alarm_t; EEPROMWrite(EEPROM_time_set_alarm,time_alarm_t,1);
#ifdef DEBUG
	send_string_to_UART3("Device: Set setting device! Settings: \n\r");
	send_string_to_UART3("SMS_AT_UNCORRECT_SMS: ");
	check_device_setting(DEVICE_SETTING_SMS_AT_UNCORRECT_SMS) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("AUTO_GUARD_AT_START: ");
	check_device_setting(DEVICE_SETTING_AUTO_GUARD_AT_START) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("SMS_AT_ALARM: ");
	check_device_setting(DEVICE_SETTING_SMS_AT_ALARM) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("CHANGE_GUARD_USE_CALL: ");
	check_device_setting(DEVICE_SETTING_CHANGE_GUARD_USE_CALL) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("BAN_OUTPUT_CALL: ");
	check_device_setting(DEVICE_SETTING_BAN_OUTPUT_CALL) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("SMS_AT_SMS_COMMAND: ");
	check_device_setting(DEVICE_SETTING_SMS_AT_SMS_COMMAND) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("SMS_AT_CHANGE_GUARD: ");
	check_device_setting(DEVICE_SETTING_SMS_AT_CHANGE_GUARD) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("SMS_AT_STARTUP: ");
	check_device_setting(DEVICE_SETTING_SMS_AT_STARTUP) ? send_string_to_UART3("ON \n\r") : send_string_to_UART3("OFF \n\r");

	send_string_to_UART3("\n\r Time to guard: ");
	send_int_to_UART3(time_to_guard_t);
	send_string_to_UART3(" Time alarm: ");
	send_int_to_UART3(time_alarm_t);
	send_string_to_UART3(" \n\r ");
#endif
}

uint8_t check_device_setting(uint8_t opt){
	return (device_settings & opt);
}
/////////////////////////////////                      GUARD
void guard_on(){
		led_blink_stop(OUT_MODE_GUARD);
		guard_st = GUARD_ON;
		out_on_mode(OUT_MODE_GUARD);
		changed_guard_sms(GUARD_ON);
#ifdef DEBUG_GUARD
	send_string_to_UART3("GUARD: ON! \n\r");
#endif
}

void guard_on_TM(){
	if (time_set_to_guard_on){
		if (!time_to_guard_on){
			time_to_guard_on = time_set_to_guard_on;
			led_blink(OUT_MODE_GUARD,5,5);
		}else{
			guard_off();
		}
	}else{
		if (alarm_st) alarm_off();
		else guard_on();
	}
}

void guard_off(){
	led_blink_stop(OUT_MODE_GUARD);
	time_to_guard_on = -1;
	guard_st = GUARD_OFF;
	out_off_mode(OUT_MODE_GUARD);
	alarm_off();
	clear_alarm_input();
	changed_guard_sms(GUARD_OFF);
#ifdef DEBUG_GUARD
	send_string_to_UART3("GUARD: OFF! \n\r");
#endif
}

uint8_t get_guard_st(){
	return guard_st;
}

uint8_t get_alarm_st(){
	return alarm_st;
}

void changed_guard_sms(int status){
	str_add_str(output_sms_message,sizeof(output_sms_message),(status ? "na ohrane " : "snqt s ohranQ " ),0);
	str_add_str(output_sms_message,sizeof(output_sms_message),last_control_guard, 13);
	if (last_control_guard[0]) send_sms_message_for_all(output_sms_message,SMS_FUNCTION_CHANGE_GUARD_ALARM);
	clear_last_control_guard();
	clear_output_sms_message();
}

void alarm_on(){
	out_on_mode(OUT_MODE_ALARM);
	alarm_st = ALARM_ON;

	str_add_str(output_sms_message,sizeof(output_sms_message),"srabotal vhod ",0);
	str_add_num(output_sms_message,get_alarm_input());
	str_add_str(output_sms_message,sizeof(output_sms_message),": ",0);
	add_input_text(output_sms_message, get_alarm_input());
	send_sms_message_for_all(output_sms_message,get_alarm_input());
	alarm_call();
#ifdef DEBUG_GUARD
	send_string_to_UART3("ALAAAAARM: ON! \n\r");
#endif
}

void alarm_off(){
		out_off_mode(OUT_MODE_ALARM);
		out_off_mode(OUT_MODE_LAMP);
		alarm_st = ALARM_OFF;
#ifdef DEBUG_GUARD
	send_string_to_UART3("ALAAAAARM: OFF! \n\r");
#endif
}

void out_on_mode(uint8_t mode){
	led_on_mode(mode);
	output_on_mode(mode);
}

void out_off_mode(uint8_t mode){
	led_off_mode(mode);
	output_off_mode(mode);
}

void check_battery(){
	unsigned int U_adc = ADC_read(DET_220_CHANNEL);
	unsigned int U = U_adc*181.5/4096;
	u_coef =  U * 100 / u_220V;
	if (u_coef>100) u_coef = 100;
#ifdef DEBUG_220V
	send_string_to_UART3("DETECT 220v: ");
	send_int_to_UART3(U);
	send_string_to_UART3(" \n\r");
#endif

	if ((U < (u_batt + ((u_220V - u_batt)*0.2))) && (powered == POWERED_220V)){
#ifdef DEBUG
	send_string_to_UART3("POWER: BATTARY ON!!\n\r");
#endif
		powered = POWERED_BATTERY;
		out_on_mode(OUT_MODE_220V); // измерение со входа
		str_add_str(output_sms_message,sizeof(output_sms_message),"vnimanie! otkaz 220v!",0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"\n",0);
		str_add_str(output_sms_message,sizeof(output_sms_message),"akkum: ",0);
		uint8_t u_min = u_220V - 40;
		set_timeout_7(30);
		while_timeout_7();
		U_adc =ADC_read(DET_220_CHANNEL);
		uint8_t proc = ((U - u_min)*100 / (u_batt-u_min));
		if (proc > 100) proc = 100;
		str_add_num(output_sms_message,proc);
		str_add_str(output_sms_message,sizeof(output_sms_message),"%",0);
		send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);

	}else if ((U > (u_batt + (u_220V - u_batt)*0.4)) && (powered == POWERED_BATTERY)){
		powered = POWERED_220V;
		out_off_mode(OUT_MODE_220V);
		str_add_str(output_sms_message,sizeof(output_sms_message),"220v vosstanovleno",0);
		send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);
#ifdef DEBUG
	send_string_to_UART3("POWER: 220V ON!! \n\r");
#endif
	}

//	2115 - 11.2 V
//	2185 - 11.6 V
//	2271 - 12.0 V
//	2328 - 12.3 V
//	2396 - 12.6 V
//	2465 - 13.0 V
//	2684 - 13.9 V
}

void setting_powered(uint8_t step){
	uint16_t U = ADC_read(DET_220_CHANNEL);
	uint8_t U_V = U * 181.5/4096;
	if (step == 1){
		u_220V = U_V;
		EEPROMWrite(EEPROM_POWER_220V,U_V,1);
	}else if (step == 2){
		u_batt = U_V;
		EEPROMWrite(EEPROM_POWER_BATT,U_V,1);
	}
#ifdef DEBUG
	send_string_to_UART3("POWER: SET 220V (V): ");
	send_int_to_UART3(u_220V);
	send_string_to_UART3("POWER: SET BATT (V): ");
	send_int_to_UART3(u_batt);
	send_string_to_UART3("\n\r");
#endif
}

uint8_t get_powered(){
	return powered;
}

void TM_check_time(){
	if (time_to_check_TM) time_to_check_TM --;
}

void check_TM(){
	if (time_to_check_TM || get_flag_conv()) return;
	out_off_mode(OUT_MODE_TM);
	if (!get_tm_key_number()) return;
	int current_TM = one_wire_check_keys();
	if (current_TM != ONE_WIRE_KEY_DENY) {
		time_to_check_TM = 20;
		out_on_mode(OUT_MODE_TM);
		clear_last_control_guard();
		str_add_str(last_control_guard,sizeof(last_control_guard),"tm = ",0);
		str_add_num(last_control_guard,current_TM);
//		last_control_ID_number = current_TM + 100;
		if (guard_st){
			guard_off();
		}else{
			guard_on_TM();
		}
	}else{

	}
}

void read_settings(){
		read_numbers();
		read_TM_KEY_settings();
		read_ds18x20_settings();
		read_inputs_settings();
		read_output_settings();
		read_led_settings();
		read_device_settings();
		checkValidCode(1);
}

void check_lamp_blink_time(){
	if (alarm_st){
		if (lamp_blink_time > 0) {
			out_on_mode(OUT_MODE_LAMP);
		}else{
			out_off_mode(OUT_MODE_LAMP);
		}
		lamp_blink_time--;
		if (lamp_blink_time == -5) lamp_blink_time = 5;
	}
}

void check_time_to_guard_on(){
	if (time_to_guard_on > 0){
		time_to_guard_on--;
	}
}

void clear_last_control_guard(){
	unsigned int i;
	for (i=0;i<13;i++) {
		last_control_guard[i] = 0;
	}
}

void check_guard_change(){
	if (last_control_guard[0]) new_guard_st ? guard_on() : guard_off();

	if (!time_to_guard_on){
		time_to_guard_on = -1;
		guard_on();
	}
}

void set_new_guard_st(uint8_t new_st){
	new_guard_st = new_st;
}

int checkValidCode(int Step){
	static int valid = 0;
	if (Step == 1){
	  uint32_t *uid = (uint32_t *)0x1FF80050;

	  if (UUID_1 == uid[0])
		  if (UUID_2 == uid[1])
			  if (UUID_3 == uid[2]) valid = 1;
#ifdef DEBUG
	  send_string_to_UART3("\n\rUID[0] = ");
	  send_int_to_UART3(uid[0]);
	  send_string_to_UART3(" (");
	  send_int_to_UART3(UUID_1);
	  send_string_to_UART3(")\n\rUID[1] = ");
	  send_int_to_UART3(uid[1]);
	  send_string_to_UART3(" (");
	  send_int_to_UART3(UUID_2);
	  send_string_to_UART3(")\n\rUID[2] = ");
	  send_int_to_UART3(uid[2]);
	  send_string_to_UART3(" (");
	  send_int_to_UART3(UUID_3);
	  send_string_to_UART3(")");
	  if (!valid)  send_string_to_UART3("\n\rWARNING UID UNVALID!!!!");
#endif
	}
	return valid;
}

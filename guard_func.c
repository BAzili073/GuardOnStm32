#include "guard_func.h"
#include "modem.h"
#include "my_string.h"
#include "EEPROMfunc.h"
#include "1-Wire.h"
#include "UART.h"
#include "led.h"
#include "ADC_func.h"
#include "output.h"
#include "input.h"



void guard_on_TM();
void check_battery();
void clear_last_control_guard();
void changed_guard_sms(int status);



int u_battary = 2680;

uint16_t time_to_guard_on = 0;
uint16_t time_to_check_TM = 0;
int16_t time_set_to_guard_on = 10;
uint8_t device_settings = 0;
uint8_t alarm_st = 0;
uint8_t guard_st = 0;
int8_t lamp_blink_time = 5;


 typedef struct TEL_obj{
	char number[10];
	uint8_t  access;
} TEL_obj;

TEL_obj tel[MAX_TEL_NUMBERS] ={};

char tel_number_temp[10];

uint8_t last_control_ID_number = 254;
char last_control_guard[13];



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
		guard_on();
	}
}

void guard_off(){
	led_blink_stop(OUT_MODE_GUARD);
	time_to_guard_on = 0;
	guard_st = GUARD_OFF;
	clear_alarm_input();
	out_off_mode(OUT_MODE_GUARD);
	alarm_off();
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
	str_add_str(output_sms_message,(status ? "na ohranu " : "snqt s ohranu " ));
	str_add_str(output_sms_message,last_control_guard);
	if (last_control_guard[0]) send_sms_message_for_all(output_sms_message,SMS_FUNCTION_CHANGE_GUARD_ALARM);
	clear_last_control_guard();
}

void alarm_on(){
	out_on_mode(OUT_MODE_ALARM);
	alarm_st = ALARM_ON;
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
	if (ADC_read(DET_220_CHANNEL) < (u_battary * 0.93)){
		out_on_mode(OUT_MODE_220V); // измерение со входа
	}else{
		out_off_mode(OUT_MODE_220V);
	}
#ifdef DEBUG_220V
	send_string_to_UART3("DETECT 220v: ");
	send_int_to_UART3(ADC_read(DET_220_CHANNEL));
	send_string_to_UART3(" \n\r");
#endif
//	2115 - 11.2 V
//	2185 - 11.6 V
//	2271 - 12.0 V
//	2328 - 12.3 V
//	2396 - 12.6 V
//	2465 - 13.0 V
//	2684 - 13.9 V
}



void check_TM(){
	out_off_mode(OUT_MODE_TM);
	if (!get_tm_key_number()) return;
	int current_TM = one_wire_check_keys();
	if (current_TM != ONE_WIRE_KEY_DENY) {
		time_to_check_TM = 20;
		out_on_mode(OUT_MODE_TM);
		clear_last_control_guard();
		str_add_str(last_control_guard,"TM = ");
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
	uint8_t i = 0;
	uint8_t y = 0;

//////////////////   			 READ NUMBER
	for (i = 0;i< MAX_TEL_NUMBERS;i++){
		for (y = 0;y < 11; y++)
		tel[i].number[y] = EEPROMRead((EEPROM_tel_numbers + (i * 11) + y),1);
	}

//////////////////   			 READ TM ID

		read_TM_KEY_settings();

//////////////////   			 READ DS18b20 ID
		read_ds18x20_settings();

//////////////////

//////////////////

//////////////////

//////////////////

//////////////////


}

void check_lamp_blink(){
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
	if (time_to_guard_on){
		time_to_guard_on--;
		if (!time_to_guard_on) guard_on();
	}
}

void clear_last_control_guard(){
	unsigned int i;
	for (i=0;i<13;i++) {
		last_control_guard[i] = 0;
	}
}

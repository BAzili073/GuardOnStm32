#include "output.h"
#include "EEPROMfunc.h"

typedef struct OUTPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	uint16_t  time_on;
} OUTPUT_obj;

OUTPUT_obj output[MAX_OUTPUT] = {
		[0] = {.port = OUTPUT_1_PORT, .pin = OUTPUT_1, .time_on = 0, .mode = OUT_MODE_LAMP,},
		[1] = {.port = OUTPUT_2_PORT, .pin = OUTPUT_2, .time_on = 0, .mode = OUT_MODE_GUARD,},
		[2] = {.port = OUTPUT_3_PORT, .pin = OUTPUT_3, .time_on = 0, .mode = OUT_MODE_ALARM,},
		[3] = {.port = OUTPUT_4_PORT, .pin = OUTPUT_4, .time_on = 0, .mode = OUT_MODE_HAND,},
		[4] = {.port = OUTPUT_5_PORT, .pin = OUTPUT_5, .time_on = 0, .mode = OUT_MODE_HAND,},
};

void check_time_output_on(){
	int i;
	for (i = 0;i<MAX_OUTPUT;i++){
		if (output[i].time_on > 0){
			output[i].time_on--;
			if (!output[i].time_on) output_off(i+1);
		}
	}
}

void sms_control_output(uint8_t number, uint32_t state){
	if (state > 0){
		output_on(number);
		if (state > 1){
			output[number-1].time_on = state;
		}
	}else if (state == 0)  {
		output_off(number);
	}
}

void read_output_settings(){
	 int i;
	 for (i = 0;i < MAX_OUTPUT;i++){
		 uint8_t temp;
		 temp = EEPROMRead((EEPROM_output_mode + i),1);
		 if (temp != 0xFE) output[i].mode = temp;
	 }
}

void set_output_settings(uint8_t output_t, uint8_t mode_t){
	if ((mode_t >= '0') && (mode_t < '9')) mode_t = mode_t - '0';
	output[output_t].mode = mode_t; EEPROMWrite((EEPROM_output_mode + output_t),output[output_t].mode,1);
#ifdef DEBUG
	send_string_to_UART3("Device: Set setting output! ID:");
	send_int_to_UART3(output_t);
	send_string_to_UART3(" Mode: ");
	if ((mode_t >= 0) & (mode_t <10)) send_int_to_UART3(mode_t);
	else send_char_to_UART3(mode_t);
	send_string_to_UART3(" \n\r ");
#endif
}

void output_on(uint8_t output_t){
	GPIO_HIGH(output[output_t-1].port,(output[output_t-1].pin));
}

void output_off(uint8_t output_t){
	GPIO_LOW((output[output_t-1].port),(output[output_t-1].pin));
}

void output_on_mode(uint8_t mode){
	for (int i = 1;i<=MAX_OUTPUT;i++){
		if (output[i-1].mode == mode) output_on(i);
	}
}

void output_off_mode(uint8_t mode){
	for (int i = 1;i<=MAX_OUTPUT;i++){
			if (output[i-1].mode == mode) output_off(i);
	}
}

void output_on_hand(uint8_t output_t){
	if (output[output_t].mode == OUT_MODE_HAND) output_on(output_t);
}

void output_off_hand(uint8_t output_t){
	if (output[output_t].mode == OUT_MODE_HAND) output_off(output_t);
}

void control_out_by_id_and_mode(uint8_t output_t,uint8_t mode,uint8_t state){
	if (output[output_t].mode == mode){
		state ? output_on(output_t) : output_off(output_t);
	}
}

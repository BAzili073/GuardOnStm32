#include "output.h"
#include "EEPROMfunc.h"

typedef struct OUTPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
} OUTPUT_obj;

OUTPUT_obj output[MAX_OUTPUT] = {
		[0] = {.port = OUTPUT_1_PORT, .pin = OUTPUT_1, .mode = OUT_MODE_LAMP,},
		[1] = {.port = OUTPUT_2_PORT, .pin = OUTPUT_2, .mode = OUT_MODE_GUARD,},
		[2] = {.port = OUTPUT_3_PORT, .pin = OUTPUT_3, .mode = OUT_MODE_TM,},
		[3] = {.port = OUTPUT_4_PORT, .pin = OUTPUT_4, .mode = OUT_MODE_LAMP,},
		[4] = {.port = OUTPUT_5_PORT, .pin = OUTPUT_5, .mode = OUT_MODE_LAMP,},
};

void read_output_settings(){
	 int i;
	 for (i = 0;i < MAX_OUTPUT;i++){
		 uint8_t temp;
		 temp = EEPROMRead((EEPROM_output_mode + i),1);
		 if (temp != 0xFE) output[i].mode = temp;
	 }
}

void set_output_settings(uint8_t output_t, uint8_t mode_t){
	output[output_t].mode = mode_t; EEPROMWrite((EEPROM_output_mode + output_t),output[output_t].mode,1);
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

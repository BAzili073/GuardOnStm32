#include "output.h"


typedef struct OUTPUT_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
} OUTPUT_obj;

OUTPUT_obj output[OUTPUT_MAX] = {
		[0] = {.port = OUTPUT_1_PORT, .pin = OUTPUT_1, .mode = OUT_MODE_LAMP,},
		[1] = {.port = OUTPUT_2_PORT, .pin = OUTPUT_2, .mode = OUT_MODE_GUARD,},
		[2] = {.port = OUTPUT_3_PORT, .pin = OUTPUT_3, .mode = OUT_MODE_TM,},
		[3] = {.port = OUTPUT_4_PORT, .pin = OUTPUT_4, .mode = OUT_MODE_LAMP,},
		[4] = {.port = OUTPUT_5_PORT, .pin = OUTPUT_5, .mode = OUT_MODE_LAMP,},
};


void output_on(uint8_t output_t){
	GPIO_HIGH(output[output_t-1].port,(output[output_t-1].pin));

}

void output_off(uint8_t output_t){
	GPIO_LOW((output[output_t-1].port),(output[output_t-1].pin));
}

void output_on_mode(uint8_t mode){
	for (int i = 1;i<=OUTPUT_MAX;i++){
		if (output[i-1].mode == mode) output_on(i);
	}
}

void output_off_mode(uint8_t mode){
	for (int i = 1;i<=OUTPUT_MAX;i++){
			if (output[i-1].mode == mode) output_off(i);
	}
}

void output_on_hand(uint8_t output_t){
	if (output[output_t].mode == OUT_MODE_HAND) output_on(output_t);
}

void output_off_hand(uint8_t output_t){
	if (output[output_t].mode == OUT_MODE_HAND) output_off(output_t);
}

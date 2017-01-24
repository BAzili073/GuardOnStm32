#include "led.h"
#include "EEPROMfunc.h"

typedef struct LED_obj{
	GPIO_TypeDef * port;
	uint16_t  pin;
	uint8_t mode;
	int8_t  blink_time;
	int8_t  blink_time_on;
	int8_t  blink_time_off;
} LED_obj;

LED_obj led[MAX_LED] = {
		[0] = { .port = LED_1_PORT, .pin = LED_1, .mode = OUT_MODE_LAMP,	.blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[1] = { .port = LED_2_PORT, .pin = LED_2, .mode = OUT_MODE_GUARD, 	.blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[2] = { .port = LED_3_PORT, .pin = LED_3, .mode = OUT_MODE_TM, 		.blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[3] = { .port = LED_4_PORT, .pin = LED_4, .mode = OUT_MODE_GSM, 	.blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
		[4] = { .port = LED_5_PORT, .pin = LED_5, .mode = OUT_MODE_ALARM,	.blink_time = 0, .blink_time_on = -127, .blink_time_off = -127},
};

void read_led_settings(){
	 int i;
	 for (i = 0;i < MAX_LED;i++){
		 uint8_t temp;
		 temp = EEPROMRead((EEPROM_led_mode + i),1);
		 if (temp != 0xFE) led[i].mode = temp;
	 }
}

void set_led_settings(uint8_t led_t, uint8_t mode_t){
	led[led_t].mode = mode_t; EEPROMWrite((EEPROM_led_mode + led_t),led[led_t].mode,1);
}

void led_on(int id){
	GPIO_HIGH(led[id].port,(led[id].pin));
}

void led_off(int id){
	GPIO_LOW(led[id].port,(led[id].pin));
}

void led_on_mode(uint8_t mode){
	int i;
	for (i = 0;i<MAX_LED;i++){
		if (led[i].mode == mode) led_on(i);
	}
}

void led_off_mode(uint8_t mode){
	int i;
	for (i = 0;i<MAX_LED;i++){
		if (led[i].mode == mode) led_off(i);
	}
}


void led_blink(uint8_t mode, int8_t time_on,int8_t time_off){
	int i;
	for (i = 0;i < MAX_LED;i++){
		if (led[i].mode == mode){
			led[i].blink_time_on = time_on;
			led[i].blink_time_off = time_off;
		}
	}
}

void led_blink_stop(uint8_t mode){
	int i;
	for (i = 0;i < MAX_LED;i++){
		if (led[i].mode == mode){
			led[i].blink_time_on = LED_BLINK_STOP;
		}
	}
	led_off_mode(mode);
}

void check_led_blink(){
	int i;
	for (i = 0; i<MAX_LED;i++){
		if (led[i].blink_time_on != LED_BLINK_STOP){
			led[i].blink_time --;
			if (led[i].blink_time > 0){
				led_on(i);
			}else{
				led_off(i);
				if (led[i].blink_time <= (led[i].blink_time_off * (-1))) led[i].blink_time = led[i].blink_time_on+1;
			}
		}
	}
}

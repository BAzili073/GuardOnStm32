#ifdef LED
#else
#define LED

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "defines.h"

void led_on(int id);
void led_off(int id);
void led_blink(uint8_t led_mode, int8_t time_on,int8_t time_off);
void led_blink_stop(uint8_t led_mode);
void led_on_mode(uint8_t mode);
void led_off_mode(uint8_t mode);
void check_led_blink();
void read_led_settings();
void set_led_settings(uint8_t led_t, uint8_t mode_t);

#endif

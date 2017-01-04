#ifdef INPUT
#else
#define INPUT

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "defines.h"


 int check_input(int input_t);
void check_inputs();
void clear_alarm_input();
int get_alarm_input();
void add_input_text(char * str, int inp);
void check_time_to_alarm();
void read_inputs_settings();
void set_time_to_alarm(uint8_t time_to_alarm_t);
void set_input_text(uint8_t inp, char * text_t);
void set_input_settings(uint8_t inp, uint8_t v_max_t, uint8_t v_min_t,uint8_t mode_t,uint8_t time_to_alarm_t);
#endif

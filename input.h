#ifdef INPUT
#else
#define INPUT

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "defines.h"


 int check_input(int input_t);
void check_inputs();
void clear_alarm_input();
void check_time_to_alarm();
void read_inputs_settings();
#endif

#ifdef MAIN_GUARD_C
#else
#define MAIN_GUARD_C

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "modem_module.h"
#include "my_string.h"
#include "defines.h"
#include "EEPROMfunc.h"
#include "guard_func.h"
#include "1-Wire.h"
#include "input.h"

void main_guard(){
	check_TM();
	check_guard_change();
	check_temperature();
	check_inputs();
	check_battery();
	check_send_report();
}










#endif

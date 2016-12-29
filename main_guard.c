#ifdef MAIN_GUARD_C
#else
#define MAIN_GUARD_C

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "modem.h"
#include "my_string.h"
#include "defines.h"
#include "EEPROMfunc.h"
#include "guard_func.h"
#include "1-Wire.h"

void main_guard(){
	if (!time_to_check_TM) {
		check_TM();
	}
	check_temperature();
	check_inputs();
	check_battery();
	check_gsm_message();
}










#endif

#ifdef MAIN_GUARD_C
#else
#define MAIN_GUARD_C

#include "defines.h"
#include INCLUDED_FILE_1
#include INCLUDED_FILE_2
#include "modem_module.h"
#include "my_string.h"
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

#ifdef OUTPUT
#else
#define OUTPUT


#include "defines.h"
#include INCLUDED_FILE_1
#include INCLUDED_FILE_2


void output_on_mode(uint8_t mode);
void output_off_mode(uint8_t mode);
void output_on_hand(uint8_t output);
void output_off_hand(uint8_t output);
void control_out_by_id_and_mode(uint8_t output_t,uint8_t mode,uint8_t state);
void read_output_settings();
void set_output_settings(uint8_t output_t, uint8_t mode_t);
void sms_control_output(uint8_t number, uint32_t state);
void check_time_output_on();




#endif

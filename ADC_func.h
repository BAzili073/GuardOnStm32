#ifdef ADC_func
#else
#define ADC_func

#include "defines.h"
#include INCLUDED_FILE_1
#include INCLUDED_FILE_2


void ADC_init();
unsigned int ADC_read(uint32_t chanel);



#endif

#ifdef ADC_func
#else
#define ADC_func

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"



void ADC_init();
unsigned int ADC_read(uint32_t chanel);



#endif

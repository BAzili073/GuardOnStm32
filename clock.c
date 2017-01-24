#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"


void set_core_clock(){

	  FLASH->ACR = FLASH_ACR_ACC64;
	  FLASH->ACR|=FLASH_ACR_LATENCY;
	  FLASH->ACR|=FLASH_ACR_PRFTEN;

	RCC -> CR |= RCC_CR_HSION;
	while(!(RCC->CR&RCC_CR_HSIRDY));
	RCC -> CFGR |= RCC_CFGR_SW_HSI;
//	RCC -> CFGR &= ~RCC_CFGR_SW_PLL;
		//	RCC -> CR &= ~RCC_CR_PLLON;
		//	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV8;

		//	RCC -> CR &= ~RCC_CR_PLLON;
		//	RCC -> CFGR |= RCC_CFGR_PLLMULL6;
		//	RCC -> CFGR &= ~RCC_CFGR_PLLSRC;
		//	RCC -> CFGR |= RCC_CFGR_SW_PLL;
		//	RCC -> CR |= RCC_CR_PLLON;
		//	while((RCC->CR & RCC_CR_PLLRDY) == 0){};
		//	RCC -> CFGR |= RCC_CFGR_SW_PLL;
}




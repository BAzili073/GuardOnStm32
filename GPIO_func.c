#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"

void PORTA_init();
void PORTB_init();
void PORTC_init();
void PORTD_init();

void GPIO_init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

		GPIO_InitTypeDef initSrtuct;


		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_OD;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = GPIO_PIN_11;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( GPIOA, &initSrtuct);



		//UART1
		initSrtuct.Alternate = GPIO_AF7_USART1;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( GPIOA, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = GPIO_PIN_1 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( GPIOA, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_INPUT;
		initSrtuct.Pin = (GPIO_PIN_0 | GPIO_PIN_4);
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( GPIOA, &initSrtuct);

		RCC->AHBENR |= RCC_AHBENR_GPIOCEN;


			initSrtuct.Alternate = 0;
			initSrtuct.Mode = GPIO_MODE_ANALOG;
			initSrtuct.Pull = GPIO_NOPULL;
			initSrtuct.Pin = GPIO_PIN_0 ;
			initSrtuct.Speed = GPIO_SPEED_HIGH;
			HAL_GPIO_Init( GPIOC, &initSrtuct);

			initSrtuct.Alternate = 0;
			initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
			initSrtuct.Pin = (GPIO_PIN_9 | GPIO_PIN_8) ;
			initSrtuct.Speed = GPIO_SPEED_HIGH;
			HAL_GPIO_Init( GPIOC, &initSrtuct);

			initSrtuct.Alternate = 0;
			initSrtuct.Mode = GPIO_MODE_OUTPUT_OD;
			initSrtuct.Pin = MODEM_ON_PIN ;
			initSrtuct.Speed = GPIO_SPEED_HIGH;
			HAL_GPIO_Init( MODEM_ON_PORT, &initSrtuct);
			GPIO_HIGH(MODEM_ON_PORT,MODEM_ON_PIN);

}

void PORTA_init(){


}

void PORTB_init(){

}

void PORTC_init(){



}

void PORTD_init(){

}

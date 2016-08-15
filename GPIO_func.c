#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"

void PORTA_init();
void PORTB_init();
void PORTC_init();
void PORTD_init();

void init_one_wire_input(){
	GPIO_InitTypeDef initSrtuct;

	initSrtuct.Alternate = 0;
	initSrtuct.Mode = GPIO_MODE_INPUT;
	initSrtuct.Pin = ONE_WIRE_PIN;
	initSrtuct.Pull = GPIO_NOPULL;
	initSrtuct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ONE_WIRE_PORT, &initSrtuct);
}

void init_one_wire_output(){
	GPIO_InitTypeDef initSrtuct;

	initSrtuct.Alternate = 0;
	initSrtuct.Mode = GPIO_MODE_OUTPUT_OD;
	initSrtuct.Pin = ONE_WIRE_PIN;
	initSrtuct.Pull = GPIO_NOPULL;
	initSrtuct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ONE_WIRE_PORT, &initSrtuct);
}

void GPIO_init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

		GPIO_InitTypeDef initSrtuct;


	init_one_wire_output();

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

//		initSrtuct.Alternate = 0;
//		initSrtuct.Mode = GPIO_MODE_INPUT;
//		initSrtuct.Pin = (GPIO_PIN_0 | GPIO_PIN_4);
//		initSrtuct.Speed = GPIO_SPEED_HIGH;
//		HAL_GPIO_Init( GPIOA, &initSrtuct);




		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pin = (LED_7 | LED_8) ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( GPIOB, &initSrtuct);





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

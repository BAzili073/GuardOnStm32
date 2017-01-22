#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "modem.h"
#include "UART.h"

void init_one_wire_output(){
	GPIO_InitTypeDef initSrtuct;
	initSrtuct.Alternate = 0;
//		initSrtuct.Mode = GPIO_MODE_INPUT;
	initSrtuct.Mode = GPIO_MODE_OUTPUT_OD;
	initSrtuct.Pin = ONE_WIRE_PIN;
	initSrtuct.Pull = GPIO_NOPULL;
	initSrtuct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ONE_WIRE_PORT, &initSrtuct);
	GPIO_LOW(ONE_WIRE_PORT,ONE_WIRE_PIN);
}
void init_one_wire_input(){
	GPIO_InitTypeDef initSrtuct;
	initSrtuct.Alternate = 0;
//		initSrtuct.Mode = GPIO_MODE_INPUT;
	initSrtuct.Mode = GPIO_MODE_OUTPUT_OD;
	initSrtuct.Pin = GPIO_PIN_15;
//	initSrtuct.Pull = GPIO_NOPULL;
	initSrtuct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &initSrtuct);
}
void GPIO_init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

		GPIO_InitTypeDef initSrtuct;

		/////////////////////////////////////////////////////////// ONE WIRE
		initSrtuct.Alternate = 0;
//		initSrtuct.Mode = GPIO_MODE_INPUT;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_OD;
		initSrtuct.Pin = ONE_WIRE_PIN;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(ONE_WIRE_PORT, &initSrtuct);
		GPIO_HIGH(ONE_WIRE_PORT,ONE_WIRE_PIN);

		//////////////////////////////////////////////////////////////////////UART1
		initSrtuct.Alternate = GPIO_AF7_USART1;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = UART1_PIN_RX | UART1_PIN_TX;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( UART1_PORT, &initSrtuct);


		/////////////////////////////////////////////////////////////////////UART2
		initSrtuct.Alternate = GPIO_AF7_USART2;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = UART2_PIN_RX | UART2_PIN_TX;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( UART2_PORT, &initSrtuct);

		/////////////////////////////////////////////////////////////////////UART3
		initSrtuct.Alternate = GPIO_AF7_USART3;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = UART3_PIN_RX | UART3_PIN_TX;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( UART3_PORT, &initSrtuct);


		/////////////////////////////////////////////////////////////////LEDS
		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pin = LED_1 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( LED_1_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pin = LED_2 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( LED_2_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pin = LED_3 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( LED_3_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pin = LED_4 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( LED_4_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pin = LED_5 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( LED_5_PORT, &initSrtuct);


		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_INPUT;
		initSrtuct.Pin = DEVICE_RESET_PIN;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( DEVICE_RESET_PORT, &initSrtuct);
//
//		initSrtuct.Alternate = 0;
//		initSrtuct.Mode = GPIO_MODE_IT_RISING_FALLING;
//		initSrtuct.Pin = GPIO_PIN_0 | GPIO_PIN_11 | GPIO_PIN_12;
//		initSrtuct.Speed = GPIO_SPEED_HIGH;
//		HAL_GPIO_Init( GPIOA, &initSrtuct);
//
//
//		initSrtuct.Alternate = 0;
//		initSrtuct.Mode = GPIO_MODE_IT_RISING_FALLING;
//		initSrtuct.Pin = GPIO_PIN_9 ;
//		initSrtuct.Speed = GPIO_SPEED_HIGH;
//		HAL_GPIO_Init( GPIOB, &initSrtuct);


		/////////////////////////////////////////////////////////// INPUTS


		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_ANALOG;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = (INPUT_1 | INPUT_2 | INPUT_3 | INPUT_4 | INPUT_5);
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( INPUT_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_INPUT;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = B_INPUT_1 | B_INPUT_2 | B_INPUT_3;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( B_INPUT_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_INPUT;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = OPEN_CAP_PIN;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( OPEN_CAP_PORT, &initSrtuct);
///////////////////////////////////////////////////////////////////OUTPUTS
		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = OUTPUT_1 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( OUTPUT_1_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = OUTPUT_2 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( OUTPUT_2_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = OUTPUT_3 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( OUTPUT_3_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = OUTPUT_4 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( OUTPUT_4_PORT, &initSrtuct);

		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = OUTPUT_5 ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( OUTPUT_5_PORT, &initSrtuct);
///////////////////////////////////////////////////////////////////////MODEM ON
		initSrtuct.Alternate = 0;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = MODEM_ON_PIN;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init( MODEM_ON_PORT, &initSrtuct);
		GPIO_HIGH(MODEM_ON_PORT,MODEM_ON_PIN);

}


////
void GPIO_interrupt_init(){
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG -> EXTICR[1] |= SYSCFG_EXTICR1_EXTI0_PA;
	SYSCFG -> EXTICR[3] |= SYSCFG_EXTICR3_EXTI9_PB;
	SYSCFG -> EXTICR[3] |= SYSCFG_EXTICR3_EXTI11_PA;
	SYSCFG -> EXTICR[4] |= SYSCFG_EXTICR4_EXTI12_PA;


	EXTI -> IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR9 | EXTI_IMR_MR11 | EXTI_IMR_MR12;
	EXTI -> EMR |= EXTI_EMR_MR0 | EXTI_EMR_MR9 | EXTI_EMR_MR11 | EXTI_EMR_MR12;


	EXTI -> FTSR = 0;
	EXTI -> RTSR = 0;
	EXTI -> FTSR |= EXTI_FTSR_TR9 | EXTI_FTSR_TR11 |EXTI_FTSR_TR12;
	EXTI -> RTSR |= EXTI_RTSR_TR12 | EXTI_RTSR_TR11  | EXTI_RTSR_TR0;//EXTI_RTSR_TR0 | EXTI_RTSR_TR9  |


	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}





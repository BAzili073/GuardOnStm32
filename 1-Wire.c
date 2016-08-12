#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "TIMs.h"

void one_wire_write_bit(uint8_t bit)
{
	ONE_WIRE_PORT ->ODR &= ~ONE_WIRE_PIN;
	set_timeout(bit ? 5 : 90);
	ONE_WIRE_PORT->ODR = ONE_WIRE_PIN;
	set_timeout(bit ? 90 : 5);
}

void one_wire_write_byte(uint8_t data)
{
	for(uint8_t i = 0; i<8; i++)
		one_wire_write_bit(data>>i & 1);
}

uint8_t one_wire_read_bit()
{
	uint8_t bit = 0;
	ONE_WIRE_PORT ->ODR &= ~ONE_WIRE_PIN;
	set_timeout(2);
	ONE_WIRE_PORT->ODR = ONE_WIRE_PIN;
	set_timeout(8);
	ONE_WIRE_PORT->MODER &= ~GPIO_MODER_MODER11;
	ONE_WIRE_PORT->OTYPER &= ~GPIO_OTYPER_OT_11;
	ONE_WIRE_PORT->OTYPER |=  GPIO_OTYPER_OT_11;
	bit = (ONE_WIRE_PORT -> IDR & ONE_WIRE_PIN? 1 : 0);
	ONE_WIRE_PORT->MODER |=  GPIO_MODER_MODER11;
	ONE_WIRE_PORT->OTYPER |=  GPIO_OTYPER_OT_11;
	set_timeout(80);
	return bit;
}

void one_wire_send_presence() {
	ONE_WIRE_PORT->ODR = ONE_WIRE_PIN;
	set_timeout(50);
	ONE_WIRE_PORT ->ODR &= ~ONE_WIRE_PIN;
	set_timeout(480); //420us
	ONE_WIRE_PORT->ODR = ONE_WIRE_PIN;
}

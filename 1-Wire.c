#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "TIMs.h"
#include "GPIO_func.h"
void one_wire_low();
void one_wire_high();
char one_wire_check_line();

char one_wire_check_line(){
	init_one_wire_input();
	char a = (ONE_WIRE_PORT -> IDR & ONE_WIRE_PIN);
	init_one_wire_output();
	return a;
}

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
	ONE_WIRE_PORT->MODER |=  GPIO_MODER_MODER11_0;
	ONE_WIRE_PORT->OTYPER |=  GPIO_OTYPER_OT_11;
	set_timeout(80);
	return bit;
}

char one_wire_send_presence() {
	one_wire_low();
	set_timeout(640);
	while_timeout();
	one_wire_high();
	set_timeout(40);
	while_timeout();
	uint8_t i = 0;
	for (i = 0;i<80;i++){
		if (!one_wire_check_line()){
			while(!one_wire_check_line()){

			}
			return 1;
		}
	}
	return 0;
}

void one_wire_low(){
	GPIO_LOW(ONE_WIRE_PORT,ONE_WIRE_PIN);
}

void one_wire_high(){
	GPIO_HIGH(ONE_WIRE_PORT,ONE_WIRE_PIN);
}
void one_wire_z_state_on(){
	ONE_WIRE_PORT->MODER &= ~GPIO_MODER_MODER11_0;
	ONE_WIRE_PORT->OTYPER &= ~GPIO_OTYPER_OT_11;
}

void one_wire_z_state_off(){
	ONE_WIRE_PORT->MODER  |= GPIO_MODER_MODER11_0;
	ONE_WIRE_PORT->OTYPER |= GPIO_OTYPER_OT_11;
}



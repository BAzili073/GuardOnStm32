#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"


#include "defines.h"
#include "GPIO_func.h"
#include "clock.h"
#include "ADC_func.h"
#include "TIMs.h"
#include "guard_func.h"
#include "UART.h"
#include "1-Wire.h"
#include "modem.h"
#include "string.h"
#include "EEPROMfunc.h"

//#include <strings.h>

char temp_address[2][8] = {{0x28,0x62,0x57,0xA0,0x04,0x00,0x00,0x40},{0x28,0xB8,0xa3,0xA0,0x04,0x00,0x00,0xf2}};
char number_call[10] = {"9021201364"};
//void Delay(void) {
//volatile uint32_t i;
//    for (i=0; i != 0x80000; i++);
//}

//void one_wire_interrupt_init(){
////	AFIO -> EXTICR[1] |= AFIO_EXTICR2_EXTI4_PA;
//	RCC -> APB2ENR |= RCC_APB2RSTR_SYSCFGRST;
//	SYSCFG -> EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PB;
//	EXTI -> IMR |= EXTI_IMR_MR5;
////	EXTI -> EMR |= EXTI_EMR_MR5;
//	EXTI -> FTSR |= EXTI_FTSR_TR5;
//	NVIC_EnableIRQ(EXTI9_5_IRQn);
//}
////
//void EXTI9_5_IRQHandler(){
//	if (one_wire_check_keys()) GPIO_TOGGLE(GPIOB,GPIO_PIN_7);
//	EXTI -> PR |= EXTI_PR_PR5;
//}

int main(void) {
//	strncmp("asdfs13512", "asd413tv", 3);
//	one_wire_interrupt_init();
	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA;
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA;
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA;
	set_core_clock();
	GPIO_init();
	TIM6_init();
	ADC_init();
//    TIM2_init(); //PWM
	TIM7_init();
	UART1_init();

//	led_blink(7,1,1);
//	device_settings |= DEVICE_SETTING_SMS_AT_STARTUP;
	//MODEM_ON();
//	if (device_settings & DEVICE_SETTING_AUTO_GUARD_AT_START){
//		guard_on();
//	}
//
//	if (device_settings & DEVICE_SETTING_SMS_AT_STARTUP){
//		str_add_str(output_sms_message,"ver:5.0 ");
//		sms_command_r();
//	}


    while(1) {
//    	main_guard();
    	check_gsm_message();
    	modem_check_state();
    		int a = GPIO_READ(GPIOA,GPIO_PIN_15);
    		if (!a){
//    			 modem_call("021201364");
//    			 modem_call("061430141");
//    		modem_send_sms_message(tel_number[0],"vse horowo 9?01573 1e 3 c ");


//    		if (one_wire_start_conversion_temp()){
//    		    					for (uint8_t f = 0;f<2;f++){
//    		    						send_string_to_GSM("f");
//    		    						send_char_to_GSM(48+f);
//    		    						send_string_to_GSM(" = ");
//    									send_int_to_GSM(one_wire_read_temp_to_address(temp_address[f]));
//    									send_string_to_GSM("\n\r");
//    		    					}
//    		    				}else{
//    		    					send_string_to_GSM("no dev");
//    		    				}
    	}

		set_timeout_7(50);
		while_timeout_7();
//    	else{
//    		if (!send_command_to_GSM("AT","OK",gsm_message,1000,3000)) modem_state = MODEM_STATE_OFF;
//    	}
//    	else{
//    		modem_call("9021201364");
//    	}

//    	for (uint8_t i=0;i<100;i++){
//        	    		set_timeout(15000);
//        	    		while_timeout();
//        	    	}
//    	if (one_wire_check_keys()) GPIO_TOGGLE(GPIOB,GPIO_PIN_7);
//    				if (one_wire_start_conversion_temp()){
//    					for (uint8_t f = 0;f<2;f++){
//    						send_string_to_GSM("f");
//    						send_char_to_GSM(48+f);
//    						send_string_to_GSM(" = ");
//							send_int_to_GSM(one_wire_read_temp_to_address(temp_address[f]));
//							send_string_to_GSM("\n\r");
//    					}
//    				}else{
//    					send_string_to_GSM("no dev");
//    				}
//    	one_wire_read_temp();
//    				if (one_wire_send_presence()){
//    					one_wire_write_byte(0x33);
//    					uint8_t i;
//    					for (i=0;i<8;i++){
//    						address[i] = one_wire_read_byte();
//    					}

//    					char a = one_wire_read_bit();
//    					char b = one_wire_read_bit();
//
//    					if(a) send_string_to_GSM("1");
//						else send_string_to_GSM("0");
//
//    					if(b) send_string_to_GSM("1");
//						else send_string_to_GSM("0");
//    				uint8_t * p = one_wire_enum_next();
//    				uint8_t d = *(p++);
//        	    	if(p){
//        	    		send_string_to_GSM("ID:");
//        	    		uint8_t o;
//        	    		for (o = 0;o<8;o++){
//    							send_char_to_GSM((address[o] >> 4) + (((address[o] >> 4) >= 10) ? ('A' - 10) : '0'));
//    							send_char_to_GSM((address[o] & 0x0F) + (((address[o] & 0x0F) >= 10) ? ('A' - 10) : '0'));
//    							send_char_to_GSM(' ');
//        	    		}
////    //    	    		send_string_to_UART1(address);
//        	    		send_string_to_GSM("\n\r");
//        	    	}else{
//        	    		send_string_to_GSM("HET\n\r");
//        	    	}
//    			one_wire_enum_init(); // Начало перечисления устройств
//
//
//    			while(1){
//    				uint8_t * p = one_wire_enum_next(); // Очередной адрес
//    				        if (!p)
//    				          break;
//    				        // Вывод шестнадцатиричной записи адреса в UART и рассчёт CRC
//    				        uint8_t d = *(p++);
//    				        uint8_t family_code = d; // Сохранение первого байта (код семейства)
//    				        for (uint8_t i = 0; i < 8; i++) {
//    				          send_char_to_GSM((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
//						      send_char_to_GSM((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
//						      send_char_to_GSM(' ');
//    				          d = *(p++);
//    				        }
//    				        send_string_to_GSM("\n\r");
//    			}
//    			send_string_to_GSM("BCE\n\r");
//
//
    				}
    return 0;
};

void BusFault_Handler(){
	int x = 3 * 3;
	x++;
}

void UsageFault_Handler(){
	int x = 3 * 3;
	x++;
}

void MemManage_Handler(){
	int x = 3 * 3;
	x++;
}

void HardFault_Handler(){
__asm volatile
(
" tst lr, #4 \n"
" ite eq \n"
" mrseq r0, msp \n"
" mrsne r0, psp \n"
" ldr r1, [r0, #24] \n"
" ldr r2, handler2_address_const \n"
" bx r2 \n"
" handler2_address_const: .word prvGetRegistersFromStack \n"
);

}

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
/* These are volatile to try and prevent the compiler/linker optimising them
away as the variables never actually get used.  If the debugger won't show the
values of the variables, make them global my moving their declaration outside
of this function. */
volatile uint32_t r0;
volatile uint32_t r1;
volatile uint32_t r2;
volatile uint32_t r3;
volatile uint32_t r12;
volatile uint32_t lr; /* Link register. */
volatile uint32_t pc; /* Program counter. */
volatile uint32_t psr;/* Program status register. */

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];

    /* When the following line is hit, the variables contain the register values. */
 //   for( ;; );
}


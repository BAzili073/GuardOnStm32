#include "defines.h"
#include INCLUDED_FILE_1
#include INCLUDED_FILE_2
#include "GPIO_func.h"
#include "clock.h"
#include "guard_func.h"
#include "string.h"
#include "main_guard.h"
#include "modem_module.h"

#define STM32_UUID ((uint32_t *)0x7b747800)

void FULL_RESET(){
#ifdef DEBUG
	send_string_to_UART3("FULL RESET \n\r");
#endif
	NVIC_SystemReset();
}


void FULL_ERASE_DEVICE(){
#ifdef DEBUG
	send_string_to_UART3("SYSTEM: FULL ERASE AND RESET DEVICE!!! \n\r");
#endif
	ERASE_EEPROM();
	FULL_RESET();
}

 void add_device_check();
 void add_device_mode();

//char temp_address[2][8] = {{0x28,0x62,0x57,0xA0,0x04,0x00,0x00,0x40},{0x28,0xB8,0xa3,0xA0,0x04,0x00,0x00,0xf2}};

void EXTI0_IRQHandler(){
}

void EXTI9_5_IRQHandler(){
}

void EXTI15_10_IRQHandler(){
}

int main(void) {

//  	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA;
//	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA;
//	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA;
	set_core_clock();
	GPIO_init();
	TIM6_init();
//	ADC_init();
////    TIM2_init(); //PWM
//	TIM7_init();
//	UART1_init();
//	UART2_init();
//	UART3_init();
//
////	GPIO_interrupt_init();
////	ERASE_EEPROM();
//
//	read_settings();
//	add_device_check();
//	one_wire_start_conversion_temp();
//	modem_online();
//#ifdef DEBUG
//	send_string_to_UART3("START PROGTAMM! \n\r");
//#endif
//
//	if (check_device_setting(DEVICE_SETTING_AUTO_GUARD_AT_START)){
//		guard_on();
//	}
////
//
//
//		str_add_str(output_sms_message,sizeof(output_sms_message),"zapusk",0);
//		str_add_str(output_sms_message,sizeof(output_sms_message),"\nver:5.1 24.01.17",0);
//		str_add_str(output_sms_message,sizeof(output_sms_message),"\nur:",0);
//		str_add_num(output_sms_message,(get_gsm_signal_quality()*100/31));
//		str_add_str(output_sms_message,sizeof(output_sms_message),"%\nkl: ",0);
//		str_add_num(output_sms_message,get_tm_key_number());
//		str_add_str(output_sms_message,sizeof(output_sms_message),"\ndat:",0);
//		str_add_num(output_sms_message,get_DS18x20_count());
//		send_sms_message_for_all(output_sms_message,SMS_FUNCTION_SERVICE);
//
//	if (check_device_setting(DEVICE_SETTING_SMS_AT_STARTUP)){
//			get_all_temp();
//			sms_command_r();
//	}

//    while(1) {


//
//	main_guard();
//	modem_check_state();
//	set_timeout_7(1);
//	while_timeout_7();
//    }
//    return 0;
}
//
//
//void BusFault_Handler(){
//	int x = 3 * 3;
//	x++;
//}
//
//void UsageFault_Handler(){
//	int x = 3 * 3;
//	x++;
//}
//
//void MemManage_Handler(){
//	int x = 3 * 3;
//	x++;
//}
//
//void HardFault_Handler(){
//__asm volatile
//(
//" tst lr, #4 \n"
//" ite eq \n"
//" mrseq r0, msp \n"
//" mrsne r0, psp \n"
//" ldr r1, [r0, #24] \n"
//" ldr r2, handler2_address_const \n"
//" bx r2 \n"
//" handler2_address_const: .word prvGetRegistersFromStack \n"
//);
//
//}
//
//void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
//{
///* These are volatile to try and prevent the compiler/linker optimising them
//away as the variables never actually get used.  If the debugger won't show the
//values of the variables, make them global my moving their declaration outside
//of this function. */
//volatile uint32_t r0;
//volatile uint32_t r1;
//volatile uint32_t r2;
//volatile uint32_t r3;
//volatile uint32_t r12;
//volatile uint32_t lr; /* Link register. */
//volatile uint32_t pc; /* Program counter. */
//volatile uint32_t psr;/* Program status register. */
//
//    r0 = pulFaultStackAddress[ 0 ];
//    r1 = pulFaultStackAddress[ 1 ];
//    r2 = pulFaultStackAddress[ 2 ];
//    r3 = pulFaultStackAddress[ 3 ];
//
//    r12 = pulFaultStackAddress[ 4 ];
//    lr = pulFaultStackAddress[ 5 ];
//    pc = pulFaultStackAddress[ 6 ];
//    psr = pulFaultStackAddress[ 7 ];
//
//    /* When the following line is hit, the variables contain the register values. */
// //   for( ;; );
//}
// void add_device_check(){
//	 int counter = 0;
//	 if (!one_wire_level()){
//		 led_on(0);
//		 while((!one_wire_level()) && (counter < 20)){
//			 set_timeout_7(1);
//			 while_timeout_7();
//			 counter++;
//		 }
//		 if (one_wire_level()){
//			 led_on(1);
//			 counter = 0;
//			 while((one_wire_level()) && (counter < 20)){
//				 set_timeout_7(1);
//				 while_timeout_7();
//				 counter++;
//			 }
//			 if (!one_wire_level()){
//				 led_on(2);	set_timeout_7(5);while_timeout_7();
//				 led_on(3);	set_timeout_7(5);while_timeout_7();
//				 led_on(4);	set_timeout_7(5);while_timeout_7();
//				 led_off(0);led_off(1);led_off(2);led_off(3);led_off(4);
//				 while(!one_wire_level());
//				add_device_mode();
//			 }
//		 }else{
//			 //one wire error
//
//		 }
//	 }
// }
//
//void add_device_mode(){
//	int state_reset_input = check_input(9);
//	int reset_try = 0;
//	while(1){
//		one_wire_add_device();
//		int a = check_input(9);
//		if (!state_reset_input && a){
//			reset_try++;
//			if (reset_try >= 4) {
//			FULL_ERASE_DEVICE();
//			}
//		}
//
//		state_reset_input = a;
//	}
//}

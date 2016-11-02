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
#include "SPI.h"
#include "FingerPrint.h"

//#include <strings.h>

char temp_address[2][8] = {{0x28,0x62,0x57,0xA0,0x04,0x00,0x00,0x40},{0x28,0xB8,0xa3,0xA0,0x04,0x00,0x00,0xf2}};
char number_call[10] = {"9021201364"};
//void Delay(void) {
//volatile uint32_t i;
//    for (i=0; i != 0x80000; i++);
//}



//void EXTI9_5_IRQHandler(){
//	if (one_wire_check_keys()) GPIO_TOGGLE(GPIOB,GPIO_PIN_7);
//	EXTI -> PR |= EXTI_PR_PR5;
//}

void EXTI0_IRQHandler(){
	if (!alarm_flag[ALARM_FLAG_ACC]) alarm_flag[ALARM_FLAG_ACC] = 250;
	SPI_read_reg(0x30);
	EXTI -> PR |= EXTI_PR_PR0;
}





int main(void) {
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
	UART2_init();
	SPI1_Init();
	ADXL_setup();
	GPIO_interrupt_init();





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
    	check_alarm();
    	if (!FP_time_check){
    		FP_check();
    		FP_time_check = 3;
    	}
    	if (!modem_time_check){
    		modem_check_state();
    		modem_time_check = 30;
    	}
    	if (!temperature_time_check){
			if (ADC_read(ADC_CHANNEL_18)>3100){
				if (!alarm_flag[ALARM_FLAG_TEMPERATURE]) alarm_flag[ALARM_FLAG_TEMPERATURE] = 250;
			}
			temperature_time_check = 5;
    	}
	}
    return 0;
}

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


void check_alarm(){
	if (alarm_flag[ALARM_FLAG_ACC] == 250){
		if (modem_send_sms_message(tel_number[0],"akselerometr srabotal")){
					alarm_flag[ALARM_FLAG_ACC] = 180;
					MODEM_OFF();
		}
	}
	if (alarm_flag[ALARM_FLAG_TEMPERATURE] == 250){
		if (modem_send_sms_message(tel_number[0],"temperatura prevBwena")){
					alarm_flag[ALARM_FLAG_TEMPERATURE] = 180;
					MODEM_OFF();
		}
	}
	if (alarm_flag[ALARM_FLAG_FP_TRY] == 250 ){
		if (modem_send_sms_message(tel_number[0],"palec ne podowel 3 raza")){
					alarm_flag[ALARM_FLAG_FP_TRY] = 180;
					MODEM_OFF();
		}
	}
}

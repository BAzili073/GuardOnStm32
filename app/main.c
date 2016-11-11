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




void EXTI0_IRQHandler(){
	if (!alarm_flag[ALARM_FLAG_ACC]) alarm_flag[ALARM_FLAG_ACC] = 250;
	SPI_read_reg(0x30);
	EXTI -> PR |= EXTI_PR_PR0;
}

void EXTI9_5_IRQHandler(){
	if (EXTI -> PR & EXTI_PR_PR9){
		EXTI -> PR |= EXTI_PR_PR9;
		if (!modem_time_on){
			modem_time_on = 250;
		}
	}

}

void EXTI15_10_IRQHandler(){
	if (EXTI -> PR & EXTI_PR_PR11){
		EXTI -> PR |= EXTI_PR_PR11;
		if (GPIO_READ(GPIOA,GPIO_PIN_11)){
			FP_check_allow = 0;
		}else{
			FP_check_allow = 1;
		}
	}else if(EXTI -> PR & EXTI_PR_PR12){
		FP_time_for_rec = 60;
		EXTI -> PR |= EXTI_PR_PR12;
	}
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
	UART3_init();
	SPI1_Init();
	ADXL_setup();
	GPIO_interrupt_init();





//	EEPROMWrite(3,0,1);
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

	int y;
		for (y = 0;y < 10; y++){
		tel_number[0][y] = EEPROMRead((EEPROM_tel_numbers + y),1);
	}
    while(1) {
//    	main_guard();
    	check_gsm_message();
    	check_alarm();

    	if (FP_time_for_rec == 59){
		time_LED[LED_BLUE_FOR_TIME] = 60;
#ifdef DEBUG_FINGER
	send_string_to_UART3("FP: RECORD FINGER ON!\n\r");
#endif
//			led_blink(5,5,5);
//			led_blink(6,5,5);
			FP_time_for_rec--;
    	}





    	FP_check_function();

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
	set_timeout_7(1);
	while_timeout_7();
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
	if (modem_time_on){
		if (modem_time_on == 250){
			if (modem_send_sms_message(tel_number[0],"vklUCenie modema")){
				modem_time_on = 180;
		  }
		}
		if ((modem_time_on == 1) && (modem_state == MODEM_STATE_ONLINE)){
			ADXL_int_enable();
			MODEM_OFF();
		}
	}

	if (alarm_flag[ALARM_FLAG_TEMPERATURE] == 1){
			alarm_flag[ALARM_FLAG_TEMPERATURE]--;
	}

	if (tel_number[0][0] != 0){

#ifdef DEBUG_FINGER
		if (alarm_flag[ALARM_FLAG_ACC] == 250){
			alarm_flag[ALARM_FLAG_ACC]++;
			send_string_to_UART3("ACC: Trigger!\n\r");
		}
#endif
#ifdef DEBUG_FINGER
		if (alarm_flag[ALARM_FLAG_TEMPERATURE] == 250){
			alarm_flag[ALARM_FLAG_TEMPERATURE]++;
			send_string_to_UART3("Temperatura previwena!\n\r");
		}
#endif
#ifdef DEBUG_FINGER
		if (alarm_flag[ALARM_FLAG_FP_TRY] == 250){
			alarm_flag[ALARM_FLAG_FP_TRY]++;
			send_string_to_UART3("FP: mnogo nepravilnih popitok!\n\r");
		}
#endif

			if (alarm_flag[ALARM_FLAG_ACC] >= 250){
				ADXL_int_disable();
				if (modem_send_sms_message(tel_number[0],"akselerometr srabotal")){
							alarm_flag[ALARM_FLAG_ACC] = 10;
							modem_time_on = 10;
				}
			}
			if (alarm_flag[ALARM_FLAG_TEMPERATURE] >= 250){
				if (modem_send_sms_message(tel_number[0],"temperatura prevQwena")){
							alarm_flag[ALARM_FLAG_TEMPERATURE] = 180;
							modem_time_on = 60;
				}
			}
			if (alarm_flag[ALARM_FLAG_FP_TRY] >= 250 ){
				if (modem_send_sms_message(tel_number[0],"palec ne podowel 3 raza")){
							alarm_flag[ALARM_FLAG_FP_TRY] = 180;
							modem_time_on = 60;
				}
			}
	}
}

void clear_all_allarm(){
	alarm_flag[ALARM_FLAG_ACC] = 30;
	alarm_flag[ALARM_FLAG_TEMPERATURE] = 30;
	alarm_flag[ALARM_FLAG_FP_TRY] = 30;
	modem_errors[MODEM_ERRORS_NO_CARRIER] = 0;
	modem_errors[MODEM_ERRORS_SEND_SMS] = 0;
	modem_time_on = 0;
	MODEM_OFF();
}

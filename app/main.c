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



void Delay(void) {
volatile uint32_t i;
    for (i=0; i != 0x80000; i++);
}

//void initIntExt4(){
//	AFIO -> EXTICR[1] |= AFIO_EXTICR2_EXTI4_PA;
//	EXTI -> FTSR |= EXTI_FTSR_TR4;
//	NVIC_EnableIRQ(EXTI4_IRQn);
//	EXTI -> IMR |= EXTI_IMR_MR4;
//}
//
//void EXTI4_IRQHandler(){
//	GPIO_TOGGLE(GPIOC,GPIO_PIN_8);
//	EXTI -> PR |= EXTI_PR_PR4;
//}

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
//	led_blink(7,1,1);

    for(;;) {
    	set_timeout(10000);
    	while(!time_out);
    	one_wire_send_presence();
    	set_timeout(60);
    	while(!time_out);
    	int a = one_wire_read_bit();
    	if (a) send_string_to_GSM("YES\n\r");
    	else send_string_to_GSM("NOTHING\n\r");
//    	if (modem_state == MODEM_STATE_OFF) GSM_ON();
//    	led_blink_stop(7);
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


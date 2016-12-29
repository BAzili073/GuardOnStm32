#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "guard_func.h"
#include "1-Wire.h"
#include "modem.h"
#include "FingerPrint.h"
#include "guard_func.h"
#include "input.h"
#include "led.h"

uint8_t m_sec = 0;
int time_access_lock = 0;


volatile int timeout_7;
char state_check_term = 0;


void TIM2_init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2 -> CCER |= TIM_CCER_CC2E;
	TIM2 -> CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
	TIM2 -> CR1 |= TIM_CR1_CEN;
}

void TIM7_init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	TIM7->CR1 |= TIM_CR1_ARPE;
	TIM7->DIER |= TIM_DIER_UIE;
	TIM7 -> ARR = 100;
	TIM7 -> PSC = 16000 - 1;
	NVIC_EnableIRQ(TIM7_IRQn);
	TIM7-> CR1 |= TIM_CR1_CEN;
}

void  TIM7_IRQHandler(){

	  if (timeout_7) timeout_7 --;
	  if (modem_time_check) modem_time_check--;
	  if (FP_detect_time) FP_detect_time--;
	  if (FP_time_reset) FP_time_reset--;
	  if (FP_time_check) FP_time_check--;
	  if (time_to_check_TM) time_to_check_TM--;
	  check_lamp_blink();
	  check_led_blink();
	  m_sec++;

	  if (m_sec == 10) {
		  if (time_to_check_temp > 0) time_to_check_temp--;
		  if (time_access_lock) time_access_lock--;
		  if (FP_time_reset) FP_time_reset--;
		  if (FP_time_for_rec) FP_time_for_rec--;
		  if (modem_time_on && (modem_time_on<200)) modem_time_on--;
	  		m_sec = 0;
	  		check_time_to_alarm();
//	  		check_time_to_guard_on();
	  	}
	   TIM7 -> SR &= ~TIM_SR_UIF;
}






void TIM6_init(){
			RCC->APB1ENR |=  RCC_APB1ENR_TIM6EN ;
			TIM6->CR1 |= TIM_CR1_OPM;
//			TIM6->PSC =  16000 - 1;
			TIM6->PSC =  16-1;
}

void set_timeout(int t)
{
	TIM6->ARR = t;
	TIM6->EGR |= TIM_EGR_UG;
	TIM6->SR &= !TIM_SR_UIF;
	TIM6->CR1 |= TIM_CR1_CEN;
}

void while_timeout(){
	while(!time_out);
}

void set_timeout_7 (int m_sec){
	timeout_7 = m_sec;
}

void while_timeout_7(){
//	FP_check_function();
	while(timeout_7);
}

void reset_timer()
{
	TIM6->EGR |= TIM_EGR_UG;
	TIM6->SR &= !TIM_SR_UIF;
}

void TIM2_set_pwm_duty_cycle(uint8_t cyc){
	if (cyc > 100) cyc = 100;
	if (cyc < 0) cyc = 0;
	TIM2 -> CCR2 = (0xffff / 100 * cyc);
}


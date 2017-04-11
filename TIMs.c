#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "guard_func.h"
#include "1-Wire.h"
#include "modem_module.h"
#include "guard_func.h"
#include "input.h"
#include "led.h"
#include "DS18x20.h"
#include "TM_KEY.h"
#include "EEPROMfunc.h"

//int time_access_lock = 0;



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
	static uint8_t m_sec = 0;
	static uint8_t sec = 0;
	static uint8_t min = 0;
	static uint8_t hour = 0;
	static uint16_t day = 0;

	  if (timeout_7) timeout_7 --;
	  modem_time();

	  TM_check_time();
	  check_lamp_blink_time();
	  check_led_blink();
	  time_check_temp();
	  m_sec++;
//	  FP_time();
	  if (m_sec == 10) {
		  check_time_output_on();
		  sec++;
		  if (sec == 60){
			  sec = 0;
			  min++;
			  if (min == 60){
				  check_time_to_report();
				  check_time_to_reset();
				  min = 0;
				  hour++;
				  if (hour == 24){
//					  last_control_guard[0] = "!";
					  hour = 0;
					  day++;
					  if (day == 30){
						  day = 0;
					  }
				  }
			  }
		  }
		m_sec = 0;
		check_time_to_alarm();
		check_time_to_guard_on();
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

void reset_timer(){
	TIM6->EGR |= TIM_EGR_UG;
	TIM6->SR &= !TIM_SR_UIF;
}

void TIM2_set_pwm_duty_cycle(uint8_t cyc){
	if (cyc > 100) cyc = 100;
	if (cyc < 0) cyc = 0;
	TIM2 -> CCR2 = (0xffff / 100 * cyc);
}



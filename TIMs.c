#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "guard_func.h"
#include "1-Wire.h"
#include "modem.h"
#include "FingerPrint.h"
#include "guard_func.h"

int8_t lamp_blink_time = 5;
int8_t led_blink_time[8];
uint8_t m_sec = 0;

volatile int timeout_7;
char state_check_term = 0;

void check_time_to_alarm();
void check_lamp_blink();
void check_led_blink();
void check_time_to_guard_on();



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
	  if (FP_time_check) FP_time_check--;
	  if (temperature_time_check) temperature_time_check--;
	  check_lamp_blink();
	  check_led_blink();
	  m_sec++;

	  if (m_sec == 10) {
		  if ((alarm_flag[ALARM_FLAG_ACC]) && (alarm_flag[ALARM_FLAG_ACC] < 200)) alarm_flag[ALARM_FLAG_ACC]--;
		  if ((alarm_flag[ALARM_FLAG_TEMPERATURE]) && (alarm_flag[ALARM_FLAG_TEMPERATURE]<200)) alarm_flag[ALARM_FLAG_TEMPERATURE]--;
//		  	static int time_check_term;
//		  	time_check_term++;
//		  	if(time_check_term == TIME_FOR_CHECK_TEMP){
//		  		time_check_term = 0;
//		  		if (!state_check_term){
//		  			state_check_term = !state_check_term;
//		  			one_wire_start_conversion_temp();
//		  		}else{
//		  			state_check_term = !state_check_term;
//		  			get_all_temp();
//		  		}
//		  	}
	  		m_sec = 0;
//	  		check_time_to_alarm();
//	  		check_time_to_guard_on();
	  	}
	   TIM7 -> SR &= ~TIM_SR_UIF;
}

void check_time_to_guard_on(){
	if (time_to_guard_on){
		time_to_guard_on--;
		if (!time_to_guard_on) guard_on();
	}
}


void check_led_blink(){
	int i;
	for (i = 0; i<=7;i++){
		if (led_blink_time_on[i] != LED_BLINK_STOP){
			led_blink_time[i] --;
			if (led_blink_time[i] > 0){
				led_on_mode(i+1);
			}else{
				led_off_mode(i+1);
				if (led_blink_time[i] <= (led_blink_time_off[i] * (-1))) led_blink_time[i] = led_blink_time_on[i]+1;
			}
		}
	}
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

void check_time_to_alarm(){
		if (time_to_alarm > 0) time_to_alarm--;
		if (!time_to_alarm){
			time_to_alarm = -1;
			alarm_on();
		}
}

void check_lamp_blink(){
	if (alarm_st || time_to_guard_on){
		if (lamp_blink_time > 0) {
			output_on_mode(OUTPUT_MODE_LAMP);
		}else{
			output_off_mode(OUTPUT_MODE_LAMP);
		}
		lamp_blink_time--;
		if (lamp_blink_time == -5) lamp_blink_time = 5;
	}
}



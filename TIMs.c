#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "guard_func.h"
#include "1-Wire.h"
#include "modem.h"
#include "FingerPrint.h"
#include "guard_func.h"

int8_t lamp_blink_time = 5;
uint8_t m_sec = 0;
int time_access_lock = 0;


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
	  if (FP_time_reset) FP_time_reset--;
	  if (time_LED[LED_RED_FOR_TIME]) time_LED[LED_RED_FOR_TIME]--;
	  if (time_LED[LED_GREEN_FOR_TIME]) time_LED[LED_GREEN_FOR_TIME]--;
	  if (time_LED[LED_BLUE_FOR_TIME]) time_LED[LED_BLUE_FOR_TIME]--;
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
	for (i = 0; i<LED_MAX;i++){
		if (led[i].blink_time_on != LED_BLINK_STOP){
			led[i].blink_time --;
			if (led[i].blink_time > 0){
				led_on_mode(i+1);
			}else{
				led_off_mode(i+1);
				if (led[i].blink_time <= (led[i].blink_time_off * (-1))) led[i].blink_time = led[i].blink_time_on+1;
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
			out_on_mode(OUT_MODE_LAMP);
		}else{
			out_off_mode(OUT_MODE_LAMP);
		}
		lamp_blink_time--;
		if (lamp_blink_time == -5) lamp_blink_time = 5;
	}
}



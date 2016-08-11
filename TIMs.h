#ifdef TIM_H
#else

#define TIM_H
void TIM2_init();
void TIM7_init();
void TIM7_IRQHandler();
void TIM2_set_pwm_duty_cycle(uint8_t cyc);
void set_timeout(int t);
void reset_timer();
void TIM6_init();
void while_timeout();


#endif

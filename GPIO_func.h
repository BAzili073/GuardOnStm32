#define DEVICE_SETTING_SMS_AT_STARTUP 1<<7
#define DEVICE_SETTING_SMS_AT_CHANGE_GUARD 1<<6
#define DEVICE_SETTING_SMS_AT_SMS_COMMAND 1<<5
#define DEVICE_SETTING_BAN_OUTPUT_CALL 1<<4
#define DEVICE_SETTING_CHANGE_GUARD_USE_CALL 1<<3
#define DEVICE_SETTING_SMS_AT_ALARM 1<<2
#define DEVICE_SETTING_AUTO_GUARD_AT_START 1<<1
#define DEVICE_SETTING_SMS_AT_UNCORRECT_SMS 1<<0





void GPIO_init();
void init_one_wire_input();
void init_one_wire_output();
void GPIO_interrupt_init();
void EXTI0_IRQHandler();

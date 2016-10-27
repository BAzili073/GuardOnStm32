#ifdef DEF_H
#else

#define DEF_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"


//#define uint8_t unsigned char

//#define uint16_t unsigned int

//#define int8_t  char

//#define int16_t int


#define true 1
#define false 0
#define GPIO_HIGH(PORT, PIN) PORT->BSRR = PIN
#define GPIO_LOW(PORT, PIN) PORT->BSRR = ((PIN)<<16)
#define GPIO_READ(PORT, PIN) PORT->IDR&PIN
#define GPIO_TOGGLE(PORT, PIN) PORT->ODR ^= PIN

#define time_out ((TIM6->SR & TIM_SR_UIF) != 0)

#define DEVICE_SETTING_SMS_AT_STARTUP 1<<7
#define DEVICE_SETTING_SMS_AT_CHANGE_GUARD 1<<6
#define DEVICE_SETTING_SMS_AT_SMS_COMMAND 1<<5
#define DEVICE_SETTING_BAN_OUTPUT_CALL 1<<4
#define DEVICE_SETTING_CHANGE_GUARD_USE_CALL 1<<3
#define DEVICE_SETTING_SMS_AT_ALARM 1<<2
#define DEVICE_SETTING_AUTO_GUARD_AT_START 1<<1
#define DEVICE_SETTING_SMS_AT_UNCORRECT_SMS 1<<0

#define INPUT_1 1
#define INPUT_2 4
#define INPUT_3 5
#define INPUT_4 6
#define INPUT_5 7

#define PIN_220 10

#define OUTPUT_PORT GPIOC
#define OUTPUT_1 GPIO_PIN_4
#define OUTPUT_2 GPIO_PIN_5
#define OUTPUT_3 GPIO_PIN_6
#define OUTPUT_4 GPIO_PIN_7
#define OUTPUT_5 GPIO_PIN_8

#define GUARD_OFF 0
#define GUARD_ON 1

#define ALARM_OFF 0
#define ALARM_ON 1


//OUTPUTS_MODE
#define OUTPUT_MODE_LAMP 0
#define OUTPUT_MODE_ALARM 1
#define OUTPUT_MODE_DEVICE 2
#define OUTPUT_MODE_HAND 3
#define OUTPUT_MODE_GUARD 4
#define OUTPUT_MODE_INPUTS 95 // 95 - 99


#define LED_PORT GPIOB

#define LED_1 GPIO_PIN_0
#define LED_2 GPIO_PIN_1
#define LED_3 GPIO_PIN_2
#define LED_4 GPIO_PIN_3
#define LED_5 GPIO_PIN_4
#define LED_6 GPIO_PIN_5

#define LED_7 GPIO_PIN_4// GPIO_PIN_6
#define LED_8 GPIO_PIN_5 //GPIO_PIN_7
//DEBUG
//#define LED_7 GPIO_PIN_8
//#define LED_8 GPIO_PIN_9

#define LED_MODE_INPUT_1 1
#define LED_MODE_INPUT_2 2
#define LED_MODE_INPUT_3 3
#define LED_MODE_INPUT_4 4
#define LED_MODE_INPUT_5 5
#define LED_MODE_GSM 6
#define LED_MODE_GUARD 7

#define LED_BLINK_STOP -127


#define DEVICE_RESET_PORT GPIOA
#define DEVICE_RESET_PIN GPIO_PIN_8

#define OPEN_CAP_PORT  GPIOA
#define OPEN_CAP_PIN GPIO_PIN_0

//INPUTS_MODE
#define INPUT_MODE_NORMAL 1
#define INPUT_MODE_INVERS 0

#define INPUTS_MODE_24H 0b00000001
#define INPUTS_MODE_BUTTON_GUARD 0b00000010
#define INPUTS_MODE_INVERS 0b00000100


#define ONE_WIRE_PORT GPIOB
#define ONE_WIRE_PIN GPIO_PIN_5
#define ONE_WIRE_CRL_MODE GPIO_CRH_MODE11
#define ONE_WIRE_CRL_CNF GPIO_CRH_CNF11_0
#define ONE_WIRE_KEY_DENY 254

#define ONE_WIRE_CONVERSION_ERROR 32000
#define TIME_FOR_CHECK_TEMP 3
//GSM
#define GSM_BUFFER_SIZE 1000
#define GSM_MESSAGE_SIZE 350


#define MODEM_ON_PORT GPIOA
#define MODEM_ON_PIN GPIO_PIN_8
#define OK_ANSWER 1
#define NO_ANSWER 0

#define TEL_NUMBER_ALLOW 1
#define TEL_NUMBER_DENY 100

#define TEL_ACCESS_ADMIN 9
#define TEL_ACCESS_DOUBLE_SMS 8
#define TEL_ACCESS_CHOP 7
#define TEL_ACCESS_GUARD_ALARM 6
#define TEL_ACCESS_ALARM_5 5
#define TEL_ACCESS_ALARM_4 4
#define TEL_ACCESS_ALARM_3 3
#define TEL_ACCESS_ALARM_2 2
#define TEL_ACCESS_ALARM_1 1

#define SET_MODEM_TIME_CHECK 20

#define MODEM_STATE_OFF 0
#define MODEM_STATE_ON 1
#define MODEM_STATE_NO_SIM 2
#define MODEM_STATE_OFFLINE 3
#define MODEM_STATE_ONLINE 4
#define MODEM_STATE_SETUP 5
#define MODEM_STATE_STARTS 6

#define MODEM_ACTION_FREE 0
#define MODEM_ACTION_OUTGOING_CALL 1
#define MODEM_ACTION_SMS 3
#define MODEM_ACTION_GPRS 4
#define MODEM_ACTION_INCOMING_CALL 5
#define MODEM_ACTION_TALK 6

#define GSM_MESSAGE_UNKNOW 0
#define GSM_MESSAGE_OK 1
#define GSM_MESSAGE_ERROR 2
#define GSM_MESSAGE_INCOMING_SMS 3
#define GSM_MESSAGE_NO_SIM 4
#define GSM_MESSAGE_COPS 5
#define GSM_MESSAGE_CALL_BUSY 6
#define GSM_MESSAGE_CALL_NO_ANSWER 7

//#define SMS_FUNCTION_ALERT_1 9
//#define SMS_FUNCTION_ALERT_2 8
#define SMS_FUNCTION_SERVICE 7
#define SMS_FUNCTION_CHANGE_GUARD_ALARM 6
#define SMS_FUNCTION_ALERT_5 5
#define SMS_FUNCTION_ALERT_4 4
#define SMS_FUNCTION_ALERT_3 3
#define SMS_FUNCTION_ALERT_2 2
#define SMS_FUNCTION_ALERT_1 1
#define SMS_FUNCTION_ALERT 0





#define MAX_TEL_NUMBERS 5
#define MAX_TM 5
#define MAX_DS18x20 5

#define start_EEPROM 0x08080000
#define EEPROM_tel_numbers 			0
#define EEPROM_tel_settings 		(EEPROM_tel_numbers + (10*MAX_TEL_NUMBERS))
#define EEPROM_tms_id 				(EEPROM_tel_settings + MAX_TEL_NUMBERS)
#define EEPROM_ds18x20_id 			(EEPROM_tms_id + (8 * MAX_DS18x20))
#define EEPROM_tms_names 			(EEPROM_ds18x20_id + (8 * MAX_TM))
#define EEPROM_ds18x20_settings		(EEPROM_tms_names + (10 * MAX_TM))
#define EEPROM_device_settings		(EEPROM_ds18x20_settings + (MAX_DS18x20 * 4))
#define EEPROM_outputs_settings		(EEPROM_device_settings + 10)
#define EEPROM_inputs_settings		(EEPROM_outputs_settings + (5 * 8))
#define EEPROM_next					(EEPROM_inputs_settings + (5 * 8))



/*
 * 4096 EEPROM all
 *
 * TEL.NUMBERS : 0-10,11-21,22-32,33-43,44-54
 * TEL.NUMBERS. SETTINGS 55 - 59
 * TMs ID:
 * DS18x20 ID:
 * TM name:
 * DS18x20 SETTINGS:
 * DEVICE SETTINGS:
 * OUTPUTS SETTING:
 * INPUT SETTINGS:
 *
 */




#endif







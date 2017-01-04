#ifdef DEF_H
#else

#define DEF_H

#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

//#define DEBUG_FINGER
#define DEBUG_MODEM
//#define DEBUG_MODEM_UART
//#define DEBUG_220V
//#define DEBUG_DS18x20
//#define DEBUG_1WIRE
//#define DEBUG_ADD_DEVICE
#define DEBUG_GUARD
#define DEBUG_INPUTS
#define DEBUG

#define true 1
#define false 0
#define GPIO_HIGH(PORT, PIN) (PORT->BSRR) = (PIN)
#define GPIO_LOW(PORT, PIN) (PORT->BSRR) = ((PIN)<<16)
#define GPIO_READ(PORT, PIN) ((PORT->IDR)&PIN)
#define GPIO_TOGGLE(PORT, PIN) ((PORT)->ODR) ^= (PIN)

#define time_out ((TIM6->SR & TIM_SR_UIF) != 0)

#define MAX_INPUT 5
#define MAX_INPUT_B 3
#define MAX_OUTPUT 5

#define INPUT_PORT GPIOA
#define INPUT_1 GPIO_PIN_1
#define INPUT_2 GPIO_PIN_4
#define INPUT_3 GPIO_PIN_5
#define INPUT_4 GPIO_PIN_6
#define INPUT_5 GPIO_PIN_7

#define B_INPUT_PORT GPIOB
#define B_INPUT_1 GPIO_PIN_13
#define B_INPUT_2 GPIO_PIN_12
#define B_INPUT_3 GPIO_PIN_14


#define DET_220_PORT GPIOA
#define DET_220_PIN GPIO_PIN_0
#define DET_220_CHANNEL ADC_CHANNEL_0

#define OUTPUT_1_PORT GPIOB
#define OUTPUT_2_PORT GPIOB
#define OUTPUT_3_PORT GPIOC
#define OUTPUT_4_PORT GPIOC
#define OUTPUT_5_PORT GPIOC


#define OUTPUT_1 GPIO_PIN_8
#define OUTPUT_2 GPIO_PIN_9
#define OUTPUT_3 GPIO_PIN_13
#define OUTPUT_4 GPIO_PIN_14
#define OUTPUT_5 GPIO_PIN_15


#define GUARD_OFF 0
#define GUARD_ON 1

#define ALARM_OFF 0
#define ALARM_ON 1


//OUTPUTS_MODE
#define OUT_MODE_INPUT_1 1
#define OUT_MODE_INPUT_2 2
#define OUT_MODE_INPUT_3 3
#define OUT_MODE_INPUT_4 4
#define OUT_MODE_INPUT_5 5
#define OUT_MODE_LAMP 'l'
#define OUT_MODE_ALARM 't'
#define OUT_MODE_HAND 'r'
#define OUT_MODE_GUARD 'o'
#define OUT_MODE_GSM 'm'
#define OUT_MODE_SENSOR 'd'
#define OUT_MODE_TM 'v'
#define OUT_MODE_220V 'p'

#define MAX_LED 5
#define LED_1_PORT GPIOA
#define LED_2_PORT GPIOA
#define LED_3_PORT GPIOB
#define LED_4_PORT GPIOB
#define LED_5_PORT GPIOB


#define LED_1 GPIO_PIN_8
#define LED_2 GPIO_PIN_11
#define LED_3 GPIO_PIN_3
#define LED_4 GPIO_PIN_4
#define LED_5 GPIO_PIN_5


#define  LED_RED 3
#define  LED_BLUE 4
#define  LED_GREEN 5
//DEBUG
//#define LED_7 GPIO_PIN_8
//#define LED_8 GPIO_PIN_9

#define LED_BLINK_STOP -127


#define DEVICE_RESET_PORT GPIOA
#define DEVICE_RESET_PIN GPIO_PIN_12

#define OPEN_CAP_PORT  GPIOA
#define OPEN_CAP_PIN GPIO_PIN_15

//INPUTS_MODE
#define INPUTS_MODE_24H 0b00000001
#define INPUTS_MODE_BUTTON_GUARD 0b00000010
#define INPUTS_MODE_INVERS 0b00000100


#define ONE_WIRE_PORT GPIOB
#define ONE_WIRE_PIN GPIO_PIN_15
#define ONE_WIRE_CRL_MODE GPIO_CRH_MODE11
#define ONE_WIRE_CRL_CNF GPIO_CRH_CNF11_0
#define ONE_WIRE_KEY_DENY 254

#define ONE_WIRE_CONVERSION_ERROR 32000
#define TIME_FOR_CHECK_TEMP 3
//GSM
#define GSM_BUFFER_SIZE 1000
#define UART2_BUFFER_SIZE 260
#define UART2_MESSAGE_SIZE 26
#define GSM_MESSAGE_SIZE 350




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



//#define SMS_FUNCTION_ 9
//#define SMS_FUNCTION_ALERT_2 8
#define SMS_FUNCTION_SERVICE 7
#define SMS_FUNCTION_CHANGE_GUARD_ALARM 6
#define SMS_FUNCTION_ALERT_5 5
#define SMS_FUNCTION_ALERT_4 4
#define SMS_FUNCTION_ALERT_3 3
#define SMS_FUNCTION_ALERT_2 2
#define SMS_FUNCTION_ALERT_1 1
#define SMS_FUNCTION_ALERT 0


//#define LOCK_ACCESS_PORT GPIOC
//#define LOCK_ACCESS_PIN GPIO_PIN_13

#define MAX_TEL_NUMBERS 5
#define MAX_TM 5
#define MAX_DS18x20 5

#define TIME_CHECK_DS18B20 50
#define INPUT_TEXT_SIZE 15

#define start_EEPROM 0x08080000
#define EEPROM_device_settings		(0)
#define EEPROM_time_set_alarm		(EEPROM_device_settings + 1)
#define EEPROM_time_to_guard		(EEPROM_time_set_alarm + 1)
#define EEPROM_tel_numbers 			(EEPROM_time_to_guard + 1)
#define EEPROM_tel_access 			(EEPROM_tel_numbers + (10*MAX_TEL_NUMBERS))
#define EEPROM_tms_id 				(EEPROM_tel_access + MAX_TEL_NUMBERS)
#define EEPROM_tms_name 			(EEPROM_tms_id + (8 * MAX_TM))
#define EEPROM_ds18x20_id 			(EEPROM_tms_name + (10 * MAX_TM))
#define EEPROM_ds18x20_numbers 		(EEPROM_ds18x20_id + (8 * MAX_DS18x20))
#define EEPROM_tms_numbers 			(EEPROM_ds18x20_numbers + 1)
#define EEPROM_ds18x20_min			(EEPROM_tms_numbers + 1)
#define EEPROM_ds18x20_max			(EEPROM_ds18x20_min + MAX_DS18x20)
#define EEPROM_ds18x20_settings		(EEPROM_ds18x20_max + MAX_DS18x20)
#define EEPROM_input_v_min			(EEPROM_ds18x20_settings + MAX_DS18x20)
#define EEPROM_input_v_max			(EEPROM_input_v_min + MAX_INPUT)
#define EEPROM_input_mode			(EEPROM_input_v_max + MAX_INPUT)
#define EEPROM_input_time_to_alarm	(EEPROM_input_mode + MAX_INPUT)
#define EEPROM_input_text 			(EEPROM_input_time_to_alarm + MAX_INPUT)
#define EEPROM_output_mode			(EEPROM_input_text + MAX_INPUT * INPUT_TEXT_SIZE)
#define EEPROM_led_mode				(EEPROM_output_mode + MAX_OUTPUT)
//#define EEPROM_					(EEPROM_led_mode + MAX_OUTPUT)



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







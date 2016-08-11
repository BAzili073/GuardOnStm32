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
#define LED_7 GPIO_PIN_8
#define LED_8 GPIO_PIN_9



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

#define LED_MODE_INPUT_1 1
#define LED_MODE_INPUT_2 2
#define LED_MODE_INPUT_3 3
#define LED_MODE_INPUT_4 4
#define LED_MODE_INPUT_5 5
#define LED_MODE_GSM 6
#define LED_MODE_GUARD 7

#define LED_BLINK_STOP -127


#define ONE_WIRE_PORT GPIOA
#define ONE_WIRE_PIN GPIO_PIN_11
#define ONE_WIRE_CRL_MODE GPIO_CRH_MODE11
#define ONE_WIRE_CRL_CNF GPIO_CRH_CNF11_0


//GSM
#define GSM_BUFFER_SIZE 50
#define GSM_MESSAGE_SIZE 10


#define MODEM_ON_PORT GPIOA
#define MODEM_ON_PIN GPIO_PIN_8
#define OK_ANSWER 1
#define NO_ANSWER 0

#define MODEM_STATE_OFF 0
#define MODEM_STATE_ON 1

#define UNKNOW_GSM_MESSAGE 0
#define OK_GSM_MESSAGE 1
#define ERROR_GSM_MESSAGE 2



#define start_EEPROM 0x08080000


#endif







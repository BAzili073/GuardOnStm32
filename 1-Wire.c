#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "1-Wire.h"

#include "defines.h"
#include "TIMs.h"
#include "GPIO_func.h"
#include "UART.h"
#include "modem.h"
#include "my_string.h"





uint8_t one_wire_crc_update(uint8_t crc, uint8_t b);
uint8_t one_wire_check_crc(uint8_t address[8]);
void one_wire_low();
void one_wire_high();
char one_wire_check_line();
char one_wire_send_presence();

uint8_t one_wire_read_rom(uint8_t * buf);
uint8_t find_key(uint8_t key[8]);
int16_t one_wire_read_temp_to_address(uint8_t address[8]);
uint8_t onewire_enum[8]; // найденный восьмибайтовый адрес
uint8_t onewire_enum_fork_bit; // последний нулевой бит, где была неоднозначность (нумеруя с единицы)
int8_t time_to_check_temp = -1;
typedef struct DS18x20_obj;

DS18x20_obj DS18x20[MAX_DS18x20] = {
//		[0] = {.id = {0x28,0xB8,0xA3,0xA0,0x04,0x00,0x00,0xF2}},
//		[1] = {.id = {0x28,0x62,0x57,0xA0,0x04,0x00,0x00,0x40}},
};
typedef struct TM_KEY_obj;
TM_KEY_obj TM_KEY[MAX_TM] = {
};

char key_id[MAX_TM][8];
uint8_t ds18x20_number;
uint8_t tm_key_number;

char one_wire_level(){
	if (GPIO_READ(ONE_WIRE_PORT,ONE_WIRE_PIN)) return 1;
	else return 0;
}

void one_wire_write_bit(uint8_t bit)
{
	one_wire_low();
	if (bit){
		set_timeout(2);
		while(!time_out);
		one_wire_high();
		set_timeout(65);
		while(!time_out);
	}else{
		set_timeout(65);
		while(!time_out);
		one_wire_high();
		set_timeout(2);
		while(!time_out);
	}
}

void one_wire_write_byte(uint8_t data){
	uint8_t i;
	for(i = 8; i; i--){
		one_wire_write_bit(data & 1);
		data>>=1;
	}
}

uint8_t one_wire_read_bit() {
  one_wire_low();
  set_timeout(2);
  while(!time_out);
  one_wire_high();
  uint8_t y = 5;
  while(y){
	  y--;
  }
  uint8_t r = one_wire_level();
//  uint8_t r = 1;
//  for (y = 60;y>0;y--){
//	  if(!one_wire_level()){
//		  r = 0;
//		  y = 0;
//	  }
//  }

  set_timeout(65);
  while(!time_out);
  return r;
}



void check_temperature(){
	if (!ds18x20_number) return;
	if (time_to_check_temp == -1) {
		one_wire_start_conversion_temp();
		time_to_check_temp = 4;
		return;
	}else if (time_to_check_temp != 0){
		return;
	}
	time_to_check_temp = -1;
	int i;
	int y;
	for (i = 0;i<MAX_DS18x20;i++){
			if ((DS18x20[i].id[0] == 0x28) || (DS18x20[i].id[0] == 0x22) || (DS18x20[i].id[0] == 0x10)) {
				DS18x20[i].last_temp = one_wire_read_temp_to_address (DS18x20[i].id);
#ifdef DEBUG_DS18x20
	send_string_to_UART3("Adress: ");
	int k;
    for (uint8_t k = 0; k < 8; k++) {
    	char d = DS18x20[i].id[k];
		send_char_to_UART3((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
		send_char_to_UART3((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
		send_char_to_UART3(' ');
    }
    send_string_to_UART3("Temp: ");
    if (DS18x20[i].last_temp == ONE_WIRE_CONVERSION_ERROR){
    		send_string_to_UART3("Error read temp!  ");
    }else{
    	send_int_to_UART3((DS18x20[i].last_temp >> 4));
    	send_string_to_UART3(",");
    	send_int_to_UART3(((DS18x20[i].last_temp%16)*625)/1000);
    }
	send_string_to_UART3(" \n\r");

#endif
	if (DS18x20[i].last_temp != ONE_WIRE_CONVERSION_ERROR){
				if ((DS18x20[i].last_temp > DS18x20[i].max_temp)){
					if (DS18x20[i].alarm == DS18X20_ALARM_NORM){
						if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_OUT){
							for (y = 3;y<8;y++){
								if (DS18x20[i].settings && (1<<y)){
									if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_INVER){
										output_off_hand(y-3);
									}else{
										output_on_hand(y-3);
									}
								}
							}
						}
						if (DS18x20[i].settings && DS18X20_SETTINGS_SMS){
							str_add_str(output_sms_message,"temperatura datCika #");
											str_add_num(i,output_sms_message);
											str_add_str(output_sms_message,"vQwe normQ: ");
											str_add_num(DS18x20[i].last_temp,output_sms_message);
#ifdef DEBUG_DS18x20
		send_string_to_UART3("output_sms_message");
		send_string_to_UART3(" \n\r");
#endif
											send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
						}

						DS18x20[i].alarm = DS18X20_ALARM_UP;
					}
				}else if ((DS18x20[i].last_temp < DS18x20[i].max_temp)){
						if (DS18x20[i].alarm == DS18X20_ALARM_NORM){
							if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_OUT){
								for (y = 3;y<8;y++){
									if (DS18x20[i].settings && (1<<y)){
										if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_INVER){
											output_off_hand(y-3);
										}else{
											output_on_hand(y-3);
										}
									}
								}
							}
							if (DS18x20[i].settings && DS18X20_SETTINGS_SMS){
								str_add_str(output_sms_message,"temperatura datCika #");
								str_add_num(i,output_sms_message);
								str_add_str(output_sms_message," nije normQ: ");
								str_add_num(DS18x20[i].last_temp,output_sms_message);
		#ifdef DEBUG_DS18x20
				send_string_to_UART3("output_sms_message");
				send_string_to_UART3(" \n\r");
		#endif
								send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
							}
							DS18x20[i].alarm = DS18X20_ALARM_DOWN;
						}
				}else{
					if (DS18x20[i].alarm != DS18X20_ALARM_NORM){
						if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_OUT){
							for (y = 3;y<8;y++){
								if (DS18x20[i].settings && (1<<y)){
									if (DS18x20[i].settings && DS18X20_SETTINGS_CONTROL_INVER){
										output_on_hand(y-3);
									}else{
										output_off_hand(y-3);
									}
								}
							}
						}
						if (DS18x20[i].settings && DS18X20_SETTINGS_SMS){
							str_add_str(output_sms_message,"temperatura datCika #");
							str_add_num(i,output_sms_message);
							str_add_str(output_sms_message," v norme: ");
							str_add_num(DS18x20[i].last_temp,output_sms_message);
		#ifdef DEBUG_DS18x20
				send_string_to_UART3("output_sms_message");
				send_string_to_UART3(" \n\r");
		#endif
							send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
						}
					DS18x20[i].alarm = DS18X20_ALARM_NORM;
					}

				}
			}
			}
		}
}


uint8_t one_wire_read_byte() {
  uint8_t r = 0;
  uint8_t p = 0;
  for ( p = 8; p; p--) {
    r >>= 1;
    if (one_wire_read_bit())
      r |= 0x80;
  }
  return r;
}

uint8_t onewire_skip_rom() {
  if (!one_wire_send_presence())
    return 0;
  one_wire_write_byte(0xCC);
  return 1;
}

uint8_t one_wire_read_rom(uint8_t * buf) {
  if (!one_wire_send_presence())
    return 0;
  one_wire_write_byte(0x33);
  for (uint8_t p = 0; p < 8; p++) {
    *(buf++) = one_wire_read_byte();
  }
  return 1;
}

char one_wire_send_presence() {
	one_wire_low();
	set_timeout(480);
	while(!time_out);
	one_wire_high();
	set_timeout(70);
	while(!time_out);
		if (!one_wire_level()){
			set_timeout(410);
			while(!time_out);
			return 1;
		}
	return 0;
}

void one_wire_low(){
	GPIO_LOW(ONE_WIRE_PORT,ONE_WIRE_PIN);
//	GPIOB -> MODER |= GPIO_MODER_MODER5_0;

}

void one_wire_high(){
	GPIO_HIGH(ONE_WIRE_PORT,ONE_WIRE_PIN);
//	GPIOB -> MODER &= ~GPIO_MODER_MODER5_0;
}




// Инициализирует процедуру поиска адресов устройств
void one_wire_enum_init() {
  for (uint8_t p = 0; p < 8; p++) {
    onewire_enum[p] = 0;
  }
  onewire_enum_fork_bit = 65; // правее правого
}

// Перечисляет устройства на шине 1-wire и получает очередной адрес.
// Возвращает указатель на буфер, содержащий восьмибайтовое значение адреса, либо NULL, если поиск завешён
uint8_t * one_wire_enum_next() {
  if (!onewire_enum_fork_bit) { // Если на предыдущем шаге уже не было разногласий
    return 0; // то просто выходим ничего не возвращая
  }
  if (!one_wire_send_presence()) {
    return 0;
  }
  uint8_t bp = 8;
  uint8_t * pprev = &onewire_enum[0];
  uint8_t prev = *pprev;
  uint8_t next = 0;

  uint8_t p = 1;
  one_wire_write_byte(0xF0);
  uint8_t newfork = 0;
  for(;;) {
    uint8_t not0 = one_wire_read_bit();
    uint8_t not1 = one_wire_read_bit();
    if (!not0) { // Если присутствует в адресах бит ноль
      if (!not1) { // Но также присустствует бит 1 (вилка)
        if (p < onewire_enum_fork_bit) { // Если мы левее прошлого правого конфликтного бита,
          if (prev & 1) {
            next |= 0x80; // то копируем значение бита из прошлого прохода
          } else {
            newfork = p; // если ноль, то запомним конфликтное место
          }
        } else if (p == onewire_enum_fork_bit) {
          next |= 0x80; // если на этом месте в прошлый раз был правый конфликт с нулём, выведем 1
        } else {
          newfork = p; // правее - передаём ноль и запоминаем конфликтное место
        }
      } // в противном случае идём, выбирая ноль в адресе
    } else {
      if (!not1) { // Присутствует единица
        next |= 0x80;
      } else { // Нет ни нулей ни единиц - ошибочная ситуация
        return 0;
      }
    }
    one_wire_write_bit(next & 0x80);
    bp--;
    if (!bp) {
      *pprev = next;
      if (p >= 64)
        break;
      next = 0;
      pprev++;
      prev = *pprev;
      bp = 8;
    } else {
      if (p >= 64)
        break;
      prev >>= 1;
      next >>= 1;
    }
    p++;
  }
  onewire_enum_fork_bit = newfork;
  return &onewire_enum[0];
}

uint8_t one_wire_check_crc(uint8_t address[8]){
	uint8_t i;
	uint8_t crc = 0;
	for (i = 0; i < 8; i++) {
	  crc = one_wire_crc_update(crc, address[i]);
	}
	return crc;
}

uint8_t one_wire_crc_update(uint8_t crc, uint8_t b) {
  for (uint8_t p = 8; p; p--) {
    crc = ((crc ^ b) & 1) ? (crc >> 1) ^ 0b10001100 : (crc >> 1);
    b >>= 1;
  }
  return crc;
}

uint8_t one_wire_skip() {
	int a = !one_wire_send_presence();
  if (!one_wire_send_presence())
    return 0;
  one_wire_write_byte(0xCC);
  return 1;
}


uint8_t one_wire_start_conversion_temp(){
	if (one_wire_skip()) { // Если у нас на шине кто-то присутствует,...
	      one_wire_write_byte(0x44);
#ifdef DEBUG_DS18x20
	    send_string_to_UART3("Start conversion! \n\r");
#endif
//	      for (uint8_t i=0;i<100;i++){
//			  set_timeout(10000);
//			  while_timeout();
//	      }
	      return 1;
	}
	return 0;
}
void get_all_temp(){
	int i;
	for (i = 0;i<ds18x20_number;i++){
		DS18x20[i].last_temp = one_wire_read_temp_to_address(DS18x20[i].id);
	}
}

int16_t one_wire_read_temp_to_address(uint8_t address[8]){
	if (one_wire_send_presence()){
		one_wire_write_byte(0x55);
		uint8_t i = 0;
		for (i = 0;i<8;i++){
			one_wire_write_byte(address[i]);
		}
			one_wire_write_byte(0xBE);
			uint8_t scratchpad[8];
			uint8_t crc = 0;
			for (uint8_t i = 0; i < 8; i++) {
				scratchpad[i] = one_wire_read_byte();
			  crc = one_wire_crc_update(crc, scratchpad[i]);
			}
			if (one_wire_read_byte() != crc) {
				return ONE_WIRE_CONVERSION_ERROR;
			} else {
			  int16_t t = (scratchpad[1] << 8) | scratchpad[0];
			  if (address[0] == 0x10) { // 0x10 - DS18S20
				// у DS18S20 значение температуры хранит 1 разряд в дробной части.
				// повысить точность показаний можно считав байт 6 (COUNT_REMAIN) из scratchpad
				t <<= 3;
				if (scratchpad[7] == 0x10) { // проверка на всякий случай
				  t &= 0xFFF0;
				  t += 12 - scratchpad[6];
				}
			  } // для DS18B20 DS1822 значение по умолчанию 4 бита в дробной части
			  return t;
			}
	}
	return ONE_WIRE_CONVERSION_ERROR;
}

void one_wire_read_temp(){
	if (one_wire_skip()) { // Если у нас на шине кто-то присутствует,...
      one_wire_write_byte(0x44); // ...запускается замер температуры на всех термодатчиках
      for (uint8_t i=0;i<100;i++){
		  set_timeout(10000);
		  while_timeout();
      }
           one_wire_enum_init(); // Начало перечисления устройств
      for(;;) {
        uint8_t * p = one_wire_enum_next(); // Очередной адрес
        if (!p)
          break;
        // Вывод шестнадцатиричной записи адреса в UART и рассчёт CRC
        uint8_t d = *(p++);
        uint8_t crc = 0;
        uint8_t family_code = d; // Сохранение первого байта (код семейства)
        for (uint8_t i = 0; i < 8; i++) {
        	send_char_to_GSM((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
		    send_char_to_GSM((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
		    send_char_to_GSM(' ');
          crc = one_wire_crc_update(crc, d);
          d = *(p++);
        }
        if (crc) {
          // в итоге должен получиться ноль. Если не так, вывод сообщения об ошибке
        	send_char_to_GSM('C');
        	send_char_to_GSM('R');
        	send_char_to_GSM('C');
        } else {
          if ((family_code == 0x28) || (family_code == 0x22) || (family_code == 0x10)) {
            // Если код семейства соответствует одному из известных...
            // 0x10 - DS18S20, 0x28 - DS18B20, 0x22 - DS1822
            // проведём запрос scratchpad'а, считая по ходу crc
            one_wire_write_byte(0xBE);
            uint8_t scratchpad[8];
            crc = 0;
            for (uint8_t i = 0; i < 8; i++) {
            	scratchpad[i] = one_wire_read_byte();
              crc = one_wire_crc_update(crc, scratchpad[i]);
            }
            if (one_wire_read_byte() != crc) {
              // Если контрольная сумма скретчпада не совпала.
            	send_char_to_GSM('~');
            	send_char_to_GSM('C');
            	send_char_to_GSM('R');
            	send_char_to_GSM('C');
            	send_char_to_GSM(crc);
            } else {
              int16_t t = (scratchpad[1] << 8) | scratchpad[0];
//              if (family_code == 0x10) { // 0x10 - DS18S20
//                // у DS18S20 значение температуры хранит 1 разряд в дробной части.
//                // повысить точность показаний можно считав байт 6 (COUNT_REMAIN) из scratchpad
//                t <<= 3;
//                if (scratchpad[7] == 0x10) { // проверка на всякий случай
//                  t &= 0xFFF0;
//                  t += 12 - scratchpad[6];
//                }
//              } // для DS18B20 DS1822 значение по умолчанию 4 бита в дробной части
//              // Вывод температуры
              send_int_to_GSM(t);
            }
          } else {
            // Неизвестное устройство
        	  send_char_to_GSM('?');
          }
        }
        send_char_to_GSM('\n');
        	send_char_to_GSM('\r');
      }
      send_char_to_GSM('.');
    } else {
    	send_char_to_GSM('-');
    }
	send_char_to_GSM('\n');
	send_char_to_GSM('\r');
    set_timeout(32000); // небольшая задержка
    while_timeout();
  }

uint8_t one_wire_check_keys(){
	if (one_wire_skip()) {
	one_wire_enum_init(); // Начало перечисления устройств
	      for(;;) {
	        uint8_t * p = one_wire_enum_next(); // Очередной адрес
	        if (!p)
	          break;
	        // Вывод шестнадцатиричной записи адреса в UART и рассчёт CRC
	        uint8_t d = *(p++);
	        uint8_t crc = 0;
	        uint8_t key[8];
	        uint8_t family_code = d; // Сохранение первого байта (код семейства)
	        for (uint8_t i = 0; i < 8; i++) {
#ifdef DEBUG_TM
	send_char_to_UART3((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
	send_char_to_UART3((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
	send_char_to_UART3(' ');
#endif

	          key[i] = d;
	          crc = one_wire_crc_update(crc, d);
	          d = *(p++);
	        }
	        if (crc) {
	          // в итоге должен получиться ноль. Если не так, вывод сообщения об ошибке
#ifdef DEBUG_TM
	send_char_to_UART3('C');
	send_char_to_UART3('R');
	send_char_to_UART3('C');
#endif

	        } else {
	          if ((family_code == 0x01) || (family_code == 0x01) || (family_code == 0x01)) {
	        	  uint8_t key_access= find_key(key);
	        	  if (key_access != ONE_WIRE_KEY_DENY) return key_access;
	          } else {
	            // Неизвестное устройство
//	        	  send_char_to_GSM('?');
	          }
	        }
#ifdef DEBUG_TM
	send_char_to_UART3 ('\n');
	send_char_to_UART3 ('\r');
#endif


	      }
	}
	      return ONE_WIRE_KEY_DENY;
}

void one_wire_add_device(){
	if (one_wire_skip()) {
	one_wire_enum_init(); // Начало перечисления устройств
	      for(;;) {
	        uint8_t * p = one_wire_enum_next(); // Очередной адрес
	        if (!p)
	          break;
	        // Вывод шестнадцатиричной записи адреса в UART и рассчёт CRC
	        uint8_t d = *(p++);
	        uint8_t crc = 0;
	        uint8_t key[8];
	        uint8_t family_code = d; // Сохранение первого байта (код семейства)
	        for (uint8_t i = 0; i < 8; i++) {
#ifdef DEBUG_ADD_DEVICE
	send_char_to_UART3((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
	send_char_to_UART3((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
	send_char_to_UART3(' ');
#endif

	          key[i] = d;
	          crc = one_wire_crc_update(crc, d);
	          d = *(p++);
	        }
	        if (crc) {
	          // в итоге должен получиться ноль. Если не так, вывод сообщения об ошибке
#ifdef DEBUG_ADD_DEVICE
	send_char_to_UART3('C');
	send_char_to_UART3('R');
	send_char_to_UART3('C');
#endif
	        } else {
	          if ((family_code == 0x01) || (family_code == 0x01) || (family_code == 0x01)) { //if this tm key
	        	  if (find_key(key) == ONE_WIRE_KEY_DENY){
	        		  add_TM_key(key);
	        	  }else{
	        		  led_on(3);
	        	  }
	        	  set_timeout_7(5);
	        	  while_timeout_7();
	          }else if ((family_code == 0x28) || (family_code == 0x22) || (family_code == 0x10)){
	        	  if (!find_ds18x20(key)){
	        		  add_DS18x20(key);
	        		  set_timeout_7(5);
	        		  while_timeout_7();
	        	  }
	          }else {
	            // Неизвестное устройство
//	        	  send_char_to_GSM('?');
	          }
	        }
#ifdef DEBUG_ADD_DEVICE
	send_char_to_UART3 ('\n');
	send_char_to_UART3 ('\r');
#endif

	led_off(3);
	led_off(4);
	      }

	}
}

uint8_t find_key(uint8_t key[8]){
	uint8_t i;
	uint8_t y;
	uint8_t ok;
	for (i = 0; i < MAX_TM;i++){
		ok = 1;
		for (y = 0;y<8;y++){
			if (key[y] != TM_KEY[i].id[y]){
				ok = 0;
				break;
			}
		}
		if (ok) return i;
//		if (key == keys[i]) return 1;
	}
	return ONE_WIRE_KEY_DENY;
}

int find_ds18x20(uint8_t id[8]){
	uint8_t i;
	uint8_t y;
	uint8_t ok;
	for (i = 0; i < MAX_DS18x20;i++){
		ok = 1;
		for (y = 0;y<8;y++){
			if (id[y] != DS18x20[i].id[y]){
				ok = 0;
				break;
			}
		}
		if (ok) return 1;
//		if (key == keys[i]) return 1;
	}
	return 0;
}


void add_TM_key(uint8_t id[8]){
	if (tm_key_number < MAX_TM){
		led_on(4);
		tm_key_number++;
		EEPROMWrite(EEPROM_tms_numbers,tm_key_number,1);
		int i;
		for (i = 0;i<8;i++){
			TM_KEY[tm_key_number-1].id[i] = id[i];
			EEPROMWrite((EEPROM_tms_id + ((tm_key_number-1)*8) + i),id[i],1);
		}
	}
}

void add_DS18x20(uint8_t id[8]){
	if (ds18x20_number < MAX_DS18x20){
		led_on(4);
		ds18x20_number++;
		EEPROMWrite(EEPROM_ds18x20_numbers,ds18x20_number,1);
		int i;
		for (i = 0;i<8;i++){
			DS18x20[ds18x20_number-1].id[i] = id[i];
			EEPROMWrite((EEPROM_ds18x20_id + ((ds18x20_number-1)*8) + i),id[i],1);
		}
	}
}

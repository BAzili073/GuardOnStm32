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
int16_t one_wire_read_temp_to_address(uint8_t address[8]);

uint8_t one_wire_read_rom(uint8_t * buf);
uint8_t find_key(uint8_t key[8]);
int16_t one_wire_read_temp_to_address(uint8_t address[8]);
uint8_t onewire_enum[8]; // ��������� �������������� �����
uint8_t onewire_enum_fork_bit; // ��������� ������� ���, ��� ���� ��������������� (������� � �������)




uint8_t ds18x20_id[MAX_DS18x20][8];
char key_id[MAX_TM][8];
int16_t last_get_temp[MAX_DS18x20];
uint8_t ds18x20_number;
uint16_t ds18x20_max_temp[MAX_DS18x20];
uint16_t ds18x20_min_temp[MAX_DS18x20];
char ds18x20_alarm[MAX_DS18x20];
char ds18x20_settings[MAX_DS18x20];





char one_wire_level(){
//	init_one_wire_input();
	char a = GPIO_READ(ONE_WIRE_PORT,ONE_WIRE_PIN);
//	init_one_wire_output();
	return a;
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
	int i;
	int y;
	for (i = 0;i<MAX_DS18x20;i++){
		last_get_temp[i] = one_wire_read_temp_to_address (ds18x20_id[i]);
		if ((last_get_temp[i] > ds18x20_max_temp)){
			if (ds18x20_alarm[i] == DS18X20_ALARM_NORM){
				if (ds18x20_settings[i] && DS18X20_SETTINGS_CONTROL_OUT){
					for (y = 3;y<8;y++){
						if (ds18x20_settings && (1<<y)){
							if (ds18x20_settings && DS18X20_SETTINGS_CONTROL_INVER){
								output_off_hand(y-3);
							}else{
								output_on_hand(y-3);
							}
						}
					}
				}
				if (ds18x20_settings[i] && DS18X20_SETTINGS_SMS){
					str_add_str(output_sms_message,"temperatura datCika #");
									str_add_num(i,output_sms_message);
									str_add_str(output_sms_message,"vQwe normQ: ");
									str_add_num(last_get_temp[i],output_sms_message);
									send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
				}

				ds18x20_alarm[i] = DS18X20_ALARM_UP;
			}
		}else if ((last_get_temp[i] < ds18x20_max_temp)){
				if (ds18x20_alarm[i] == DS18X20_ALARM_NORM){
					if (ds18x20_settings[i] && DS18X20_SETTINGS_CONTROL_OUT){
						for (y = 3;y<8;y++){
							if (ds18x20_settings && (1<<y)){
								if (ds18x20_settings && DS18X20_SETTINGS_CONTROL_INVER){
									output_off_hand(y-3);
								}else{
									output_on_hand(y-3);
								}
							}
						}
					}
					if (ds18x20_settings[i] && DS18X20_SETTINGS_SMS){
						str_add_str(output_sms_message,"temperatura datCika #");
						str_add_num(i,output_sms_message);
						str_add_str(output_sms_message," nije normQ: ");
						str_add_num(last_get_temp[i],output_sms_message);
						send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
					}
					ds18x20_alarm[i] = DS18X20_ALARM_DOWN;
				}
		}else{
			if (ds18x20_alarm[i] != DS18X20_ALARM_NORM){
				if (ds18x20_settings[i] && DS18X20_SETTINGS_CONTROL_OUT){
					for (y = 3;y<8;y++){
						if (ds18x20_settings && (1<<y)){
							if (ds18x20_settings && DS18X20_SETTINGS_CONTROL_INVER){
								output_on_hand(y-3);
							}else{
								output_off_hand(y-3);
							}
						}
					}
				}
				if (ds18x20_settings && DS18X20_SETTINGS_SMS){
					str_add_str(output_sms_message,"temperatura datCika #");
					str_add_num(i,output_sms_message);
					str_add_str(output_sms_message," v norme: ");
					str_add_num(last_get_temp[i],output_sms_message);
					send_sms_message_for_all(output_sms_message,SMS_FUNCTION_ALERT);
				}
			ds18x20_alarm[i] = DS18X20_ALARM_NORM;
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




// �������������� ��������� ������ ������� ���������
void one_wire_enum_init() {
  for (uint8_t p = 0; p < 8; p++) {
    onewire_enum[p] = 0;
  }
  onewire_enum_fork_bit = 65; // ������ �������
}

// ����������� ���������� �� ���� 1-wire � �������� ��������� �����.
// ���������� ��������� �� �����, ���������� �������������� �������� ������, ���� NULL, ���� ����� �������
uint8_t * one_wire_enum_next() {
  if (!onewire_enum_fork_bit) { // ���� �� ���������� ���� ��� �� ���� �����������
    return 0; // �� ������ ������� ������ �� ���������
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
    if (!not0) { // ���� ������������ � ������� ��� ����
      if (!not1) { // �� ����� ������������� ��� 1 (�����)
        if (p < onewire_enum_fork_bit) { // ���� �� ����� �������� ������� ������������ ����,
          if (prev & 1) {
            next |= 0x80; // �� �������� �������� ���� �� �������� �������
          } else {
            newfork = p; // ���� ����, �� �������� ����������� �����
          }
        } else if (p == onewire_enum_fork_bit) {
          next |= 0x80; // ���� �� ���� ����� � ������� ��� ��� ������ �������� � ����, ������� 1
        } else {
          newfork = p; // ������ - ������� ���� � ���������� ����������� �����
        }
      } // � ��������� ������ ���, ������� ���� � ������
    } else {
      if (!not1) { // ������������ �������
        next |= 0x80;
      } else { // ��� �� ����� �� ������ - ��������� ��������
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
  if (!one_wire_send_presence())
    return 0;
  one_wire_write_byte(0xCC);
  return 1;
}


uint8_t one_wire_start_conversion_temp(){
	if (one_wire_skip()) { // ���� � ��� �� ���� ���-�� ������������,...
	      one_wire_write_byte(0x44);
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
		last_get_temp[i] = one_wire_read_temp_to_address(ds18x20_id[i]);
	}
}

int16_t one_wire_read_temp_to_address(uint8_t address[8]){
	if (one_wire_send_presence()){
		one_wire_write_byte(0x55);
		uint8_t i = 0;
		for (i = 0;i<8;i++){
			one_wire_write_byte(address[i]);
		}
		if ((address[0] == 0x28) || (address[0] == 0x22) || (address[0] == 0x10)) {
			// ���� ��� ��������� ������������� ������ �� ���������...
			one_wire_write_byte(0xBE);
			uint8_t scratchpad[8];
			uint8_t crc = 0;;
			for (uint8_t i = 0; i < 8; i++) {
				scratchpad[i] = one_wire_read_byte();
			  crc = one_wire_crc_update(crc, scratchpad[i]);
			}
			if (one_wire_read_byte() != crc) {
				return ONE_WIRE_CONVERSION_ERROR;
			} else {
			  int16_t t = (scratchpad[1] << 8) | scratchpad[0];
			  if (address[0] == 0x10) { // 0x10 - DS18S20
				// � DS18S20 �������� ����������� ������ 1 ������ � ������� �����.
				// �������� �������� ��������� ����� ������ ���� 6 (COUNT_REMAIN) �� scratchpad
				t <<= 3;
				if (scratchpad[7] == 0x10) { // �������� �� ������ ������
				  t &= 0xFFF0;
				  t += 12 - scratchpad[6];
				}
			  } // ��� DS18B20 DS1822 �������� �� ��������� 4 ���� � ������� �����
			  return t;
			}
	  }
	}
	return ONE_WIRE_CONVERSION_ERROR;
}

void one_wire_read_temp(){
	if (one_wire_skip()) { // ���� � ��� �� ���� ���-�� ������������,...
      one_wire_write_byte(0x44); // ...����������� ����� ����������� �� ���� �������������
      for (uint8_t i=0;i<100;i++){
		  set_timeout(10000);
		  while_timeout();
      }
           one_wire_enum_init(); // ������ ������������ ���������
      for(;;) {
        uint8_t * p = one_wire_enum_next(); // ��������� �����
        if (!p)
          break;
        // ����� ����������������� ������ ������ � UART � ������� CRC
        uint8_t d = *(p++);
        uint8_t crc = 0;
        uint8_t family_code = d; // ���������� ������� ����� (��� ���������)
        for (uint8_t i = 0; i < 8; i++) {
        	send_char_to_GSM((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
		    send_char_to_GSM((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
		    send_char_to_GSM(' ');
          crc = one_wire_crc_update(crc, d);
          d = *(p++);
        }
        if (crc) {
          // � ����� ������ ���������� ����. ���� �� ���, ����� ��������� �� ������
        	send_char_to_GSM('C');
        	send_char_to_GSM('R');
        	send_char_to_GSM('C');
        } else {
          if ((family_code == 0x28) || (family_code == 0x22) || (family_code == 0x10)) {
            // ���� ��� ��������� ������������� ������ �� ���������...
            // 0x10 - DS18S20, 0x28 - DS18B20, 0x22 - DS1822
            // ������� ������ scratchpad'�, ������ �� ���� crc
            one_wire_write_byte(0xBE);
            uint8_t scratchpad[8];
            crc = 0;
            for (uint8_t i = 0; i < 8; i++) {
            	scratchpad[i] = one_wire_read_byte();
              crc = one_wire_crc_update(crc, scratchpad[i]);
            }
            if (one_wire_read_byte() != crc) {
              // ���� ����������� ����� ���������� �� �������.
            	send_char_to_GSM('~');
            	send_char_to_GSM('C');
            	send_char_to_GSM('R');
            	send_char_to_GSM('C');
            	send_char_to_GSM(crc);
            } else {
              int16_t t = (scratchpad[1] << 8) | scratchpad[0];
//              if (family_code == 0x10) { // 0x10 - DS18S20
//                // � DS18S20 �������� ����������� ������ 1 ������ � ������� �����.
//                // �������� �������� ��������� ����� ������ ���� 6 (COUNT_REMAIN) �� scratchpad
//                t <<= 3;
//                if (scratchpad[7] == 0x10) { // �������� �� ������ ������
//                  t &= 0xFFF0;
//                  t += 12 - scratchpad[6];
//                }
//              } // ��� DS18B20 DS1822 �������� �� ��������� 4 ���� � ������� �����
//              // ����� �����������
              send_int_to_GSM(t);
            }
          } else {
            // ����������� ����������
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
    set_timeout(32000); // ��������� ��������
    while_timeout();
  }

uint8_t one_wire_check_keys(){
	if (one_wire_skip()) {
	one_wire_enum_init(); // ������ ������������ ���������
	      for(;;) {
	        uint8_t * p = one_wire_enum_next(); // ��������� �����
	        if (!p)
	          break;
	        // ����� ����������������� ������ ������ � UART � ������� CRC
	        uint8_t d = *(p++);
	        uint8_t crc = 0;
	        uint8_t key[8];
	        uint8_t family_code = d; // ���������� ������� ����� (��� ���������)
	        for (uint8_t i = 0; i < 8; i++) {
//	        	send_char_to_GSM((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
//			    send_char_to_GSM((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
//			    send_char_to_GSM(' ');
	          key[i] = d;
	          crc = one_wire_crc_update(crc, d);
	          d = *(p++);
	        }
	        if (crc) {
	          // � ����� ������ ���������� ����. ���� �� ���, ����� ��������� �� ������
//	        	send_char_to_GSM('C');
//	        	send_char_to_GSM('R');
//	        	send_char_to_GSM('C');
	        } else {
	          if ((family_code == 0x01) || (family_code == 0x01) || (family_code == 0x01)) {
	        	  uint8_t key_access= find_key(key);
	        	  if (key_access != ONE_WIRE_KEY_DENY) return key_access;
	          } else {
	            // ����������� ����������
//	        	  send_char_to_GSM('?');
	          }
	        }
	      }
	}
	      return ONE_WIRE_KEY_DENY;
}

uint8_t find_key(uint8_t key[8]){
	uint8_t i;
	uint8_t y;
	uint8_t ok;
	for (i = 0; i < MAX_TM;i++){
		ok = 1;
		for (y = 0;y<8;y++){
			if (key[y] != key_id[i][y]){
				ok = 0;
				break;
			}
		}
		if (ok) return i;
//		if (key == keys[i]) return 1;
	}
	return ONE_WIRE_KEY_DENY;
}


#include "1-Wire.h"
#include "TIMs.h"
#include "my_string.h"
#include "guard_func.h"
#include "led.h"
#include "output.h"
#include "DS18x20.h"
#include "TM_KEY.h"


uint8_t one_wire_crc_update(uint8_t crc, uint8_t b);
uint8_t one_wire_check_crc(uint8_t address[8]);
void one_wire_low();
void one_wire_high();
char one_wire_check_line();
char one_wire_send_presence();
uint8_t one_wire_read_rom(uint8_t * buf);
int16_t one_wire_read_temp_to_address(uint8_t address[8]);
uint8_t onewire_enum[8]; // ��������� �������������� �����
uint8_t onewire_enum_fork_bit; // ��������� ������� ���, ��� ���� ��������������� (������� � �������)


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
			  uint16_t t = (scratchpad[1] << 8) | scratchpad[0];
			  if (address[0] == 0x10) { // 0x10 - DS18S20
				// � DS18S20 �������� ����������� ������ 1 ������ � ������� �����.
				// �������� �������� ��������� ����� ������ ���� 6 (COUNT_REMAIN) �� scratchpad
				t <<= 3;
				if (scratchpad[7] == 0x10) { // �������� �� ������ ������
				  t &= 0xFFF0;
				  t += 12 - scratchpad[6];
				}
			  } // ��� DS18B20 DS1822 �������� �� ��������� 4 ���� � ������� �����
			  t = t>>4;
			  if (t > 256) t = (t & 0xFF) - 0xFF - 1;
			  return t;
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
#ifdef DEBUG_1WIRE
        	send_char_to_UART3((d >> 4) + (((d >> 4) >= 10) ? ('A' - 10) : '0'));
        	send_char_to_UART3((d & 0x0F) + (((d & 0x0F) >= 10) ? ('A' - 10) : '0'));
        	send_char_to_UART3(' ');
#endif
          crc = one_wire_crc_update(crc, d);
          d = *(p++);
        }
        if (crc) {
          // � ����� ������ ���������� ����. ���� �� ���, ����� ��������� �� ������

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
	int counter = 0;
	if  (!one_wire_level()) {
#ifdef DEBUG
	  send_string_to_UART3("KZ on line TM!!!\n\r");
#endif
		return ONE_WIRE_KEY_DENY;
	}
	if (one_wire_skip()) {
	one_wire_enum_init(); // ������ ������������ ���������
	      for(;;) {
	        uint8_t * p = one_wire_enum_next(); // ��������� �����
	        if (!p)
	          break;
	        counter++;
	        if (counter > 64) {
	        	//KZS
#ifdef DEBUG
	  send_string_to_UART3("KZ on line TM!!!");
#endif
	        	break;
	        }
	        // ����� ����������������� ������ ������ � UART � ������� CRC
	        uint8_t d = *(p++);
	        uint8_t crc = 0;
	        uint8_t key[8];
	        uint8_t family_code = d; // ���������� ������� ����� (��� ���������)
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
	          // � ����� ������ ���������� ����. ���� �� ���, ����� ��������� �� ������
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
	            // ����������� ����������
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
	          // � ����� ������ ���������� ����. ���� �� ���, ����� ��������� �� ������
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
	            // ����������� ����������
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










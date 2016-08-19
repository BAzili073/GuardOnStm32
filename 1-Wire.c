#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"

#include "defines.h"
#include "TIMs.h"
#include "GPIO_func.h"

void one_wire_low();
void one_wire_high();
char one_wire_check_line();
char one_wire_send_presence();
uint8_t one_wire_read_rom(uint8_t * buf);

uint8_t onewire_enum[8]; // ��������� �������������� �����
uint8_t onewire_enum_fork_bit; // ��������� ������� ���, ��� ���� ��������������� (������� � �������)

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
		while_timeout();
		one_wire_high();
		set_timeout(65);
		while_timeout();
	}else{
		set_timeout(65);
		while_timeout();
		one_wire_high();
		set_timeout(2);
		while_timeout();
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
  while_timeout();
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
	while_timeout();
	one_wire_high();
	set_timeout(70);
	while_timeout();
		if (!one_wire_level()){
			set_timeout(410);
			while_timeout();
			return 1;
		}
	return 0;
}

void one_wire_low(){
	GPIO_LOW(ONE_WIRE_PORT,ONE_WIRE_PIN);
}

void one_wire_high(){
	GPIO_HIGH(ONE_WIRE_PORT,ONE_WIRE_PIN);
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

// ��������� ������������� (�����) �, ���� ������� ����������� �������,
// ��������� MATCH_ROM ��� ���������� ���������� ������� onewire_enum_next ������
// ���������� 0, ���� ������� ����������� �� ��� �������, 1 - � ���� ������
//uint8_t onewire_match_last() {
//  return onewire_match(&onewire_enum[0]);
//}



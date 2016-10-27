#ifdef SPI_H
#else
#define SPI_H

#include "stm32l1xx_hal.h"

#define SPI_CS_PORT GPIOB
#define SPI_CS_PIN GPIO_PIN_2

#define SPI_SCK_PORT GPIOA
#define SPI_SCK_PIN GPIO_PIN_5

#define SPI_MISO_PORT GPIOA
#define SPI_MISO_PIN GPIO_PIN_6

#define SPI_MOSI_PORT GPIOA
#define SPI_MOSI_PIN GPIO_PIN_7

#define SPI_PORT	SPI1


void SPI1_Init();
void SPI_writeData(uint8_t address, uint8_t dataToWrite);
uint8_t SPI_readData(uint8_t address);



void lis3dh_write_reg(char reg, char value);  // ������ � ������� � ������� reg �������� value
int16_t lis3dh_read_reg(char reg);  //������ �������� �������� reg
void lis3dh_init(); //��������� ����������� ��������� ����������������
int lis3dh_get_status(); //������ ���������� ��������
int lis3dh_get_interrupt_status(); // ������ �������� ������� ����������
int	lis3dh_get_x(); // ������ �������� ��������� �� ��� X
int	lis3dh_get_y(); // ������ �������� ��������� �� ��� Y
int	lis3dh_get_z(); // ������ �������� ��������� �� ��� Z

void lis3dh_run(char frequency_range, char low_power_mode ); // ������ ������������� � ��������� ��������� frequency_range(�� 1 �� 10),
															 //� ���������� ������(low_power_mode == 0) ��� ������ ����������� ����������������� (low_power_mode != 0)
void lis3dh_stop(); // ���������� �������������
int lis3dh_ok();  //�������� ����������� �������������
//void lis3dh_enable_click_interrupt(char treshold); //
void lis3dh_enable_interrupt(char treshold, char cfg); //��������� ���������� 1(treshold - �������� �������� ��������� �� ��� �������������,
													// ��� �������� ������� �������� ����� ������� ������������ ���������, cfg - ������� 6 ����� �������� 0x30)
void lis3dh_disable_interrupt(); // ���������� ���������� �������������


int lis3dh_get_average_x(); //������ ��������(4 ���������) �������� �������� ��������� �� ��� X
int lis3dh_get_average_y(); //������ ��������(4 ���������) �������� �������� ��������� �� ��� Y
int lis3dh_get_average_z(); //������ ��������(4 ���������) �������� �������� ��������� �� ��� Z

char lis3dh_ismoving(); //��������, ���������� �� ������ ������� ��������� �� ��������� ���������� �������

void lis3dh_get_average_all_unsign(int * rez); //��������� ������� (4 ���������) �������� �������� ��������� �� 3 ���
//void lis3dh_sort_data(int * a);
int lis3dh_calc_treshold(); //����������




#endif

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



void lis3dh_write_reg(char reg, char value);  // запись в регистр с номером reg значения value
int16_t lis3dh_read_reg(char reg);  //чтение значения регистра reg
void lis3dh_init(); //включение необходимой периферии микроконтроллера
int lis3dh_get_status(); //чтение статусного регистра
int lis3dh_get_interrupt_status(); // чтение регистра статуса прерываний
int	lis3dh_get_x(); // чтение проекции ускорения на ось X
int	lis3dh_get_y(); // чтение проекции ускорения на ось Y
int	lis3dh_get_z(); // чтение проекции ускорения на ось Z

void lis3dh_run(char frequency_range, char low_power_mode ); // запуск акселерометра в частотном диапазоне frequency_range(от 1 до 10),
															 //в нормальном режиме(low_power_mode == 0) или режиме пониженного энергопотребления (low_power_mode != 0)
void lis3dh_stop(); // выключение акселерометра
int lis3dh_ok();  //проверка доступности акселерометра
//void lis3dh_enable_click_interrupt(char treshold); //
void lis3dh_enable_interrupt(char treshold, char cfg); //включение прерывания 1(treshold - значение проекции ускорения на ось акселерометра,
													// при переходе текущих значений через которую генерируется прервание, cfg - младшие 6 битов регистра 0x30)
void lis3dh_disable_interrupt(); // выключение прерываний акселерометра


int lis3dh_get_average_x(); //чтение среднего(4 измерения) значения проекции ускорения на ось X
int lis3dh_get_average_y(); //чтение среднего(4 измерения) значения проекции ускорения на ось Y
int lis3dh_get_average_z(); //чтение среднего(4 измерения) значения проекции ускорения на ось Z

char lis3dh_ismoving(); //проверка, отличается ли модуль вектора ускорения от ускорения свободного падения

void lis3dh_get_average_all_unsign(int * rez); //получение средних (4 измерения) значений проекции ускорения на 3 оси
//void lis3dh_sort_data(int * a);
int lis3dh_calc_treshold(); //вычисление




#endif

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

void ADXL_int_enable();
void ADXL_int_disable();



#endif

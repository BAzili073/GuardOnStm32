#include "SPI.h"
#include "defines.h"

void SPI1_Init(){

		__SPI1_CLK_ENABLE();

		__GPIOA_CLK_ENABLE();


		GPIO_InitTypeDef initSrtuct;

		initSrtuct.Alternate = GPIO_AF5_SPI1;
		initSrtuct.Mode = GPIO_MODE_OUTPUT_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin =  SPI_CS_PIN ;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(SPI_CS_PORT, &initSrtuct);

		initSrtuct.Alternate = GPIO_AF5_SPI1;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_PULLUP;
		initSrtuct.Pin =  SPI_SCK_PIN;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(SPI_SCK_PORT, &initSrtuct);

		initSrtuct.Alternate = GPIO_AF5_SPI1;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = SPI_MISO_PIN;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(SPI_MISO_PORT, &initSrtuct);

		initSrtuct.Alternate = GPIO_AF5_SPI1;
		initSrtuct.Mode = GPIO_MODE_AF_PP;
		initSrtuct.Pull = GPIO_NOPULL;
		initSrtuct.Pin = SPI_MOSI_PIN;
		initSrtuct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(SPI_MOSI_PORT, &initSrtuct);

		SPI_CS_PORT->BSRR = SPI_CS_PIN;


		SPI_HandleTypeDef SPIinitStruct;
		SPIinitStruct.Instance = SPI_PORT;
		SPIinitStruct.Init.CLKPhase = SPI_PHASE_2EDGE;
		SPIinitStruct.Init.TIMode = 0;
		SPIinitStruct.Init.CRCCalculation = 0;
		SPIinitStruct.Init.CLKPolarity = SPI_POLARITY_HIGH;
		SPIinitStruct.Init.DataSize = SPI_DATASIZE_8BIT;
		SPIinitStruct.Init.Direction = SPI_DIRECTION_2LINES;
		SPIinitStruct.Init.FirstBit = SPI_FIRSTBIT_MSB;
		SPIinitStruct.Init.Mode = SPI_MODE_MASTER;
		SPIinitStruct.Init.BaudRatePrescaler = SPI_CR1_BR_2 |SPI_CR1_BR_1 | SPI_CR1_BR_0;
		SPIinitStruct.Init.NSS = SPI_NSS_SOFT;
		HAL_SPI_Init(&SPIinitStruct);
		SPI_PORT->CR1 |= SPI_CR1_SPE;
}

int16_t SPI_read_reg(char reg)
{
		SPI_CS_PORT->BSRR = SPI_CS_PIN << 16;

		while(((SPI_PORT->SR) & SPI_SR_TXE) == 0);
		SPI_PORT->DR = (reg | 0x80);
		while(((SPI_PORT->SR) & SPI_SR_TXE) == 0);
		int x = SPI_PORT->DR;

		SPI_PORT->DR = 0;
		while(((SPI_PORT->SR) & SPI_SR_RXNE) == 0);
		x = SPI_PORT->DR;

		SPI_PORT->DR = 0;
		while(((SPI_PORT->SR) & SPI_SR_RXNE) == 0);

		SPI_CS_PORT->BSRR = SPI_CS_PIN;
		int i = 0;
		for(; i < 200; i++);

		return SPI_PORT->DR;
}

void SPI_write_reg(char reg, char value)
{
		SPI_CS_PORT->BSRR = SPI_CS_PIN <<16;

		while(((SPI_PORT->SR) & SPI_SR_TXE) == 0);
		SPI_PORT->DR = reg ;
		while(((SPI_PORT->SR) & SPI_SR_TXE) == 0);
		SPI_PORT->DR = value;

		/*int i = 0;
		for(; i < 20; i++);*/
		while(((SPI_PORT->SR) & SPI_SR_BSY) != 0);
		//while(((LIS3DH_SPI_PORT->SR) & SPI_SR_TXE) == 0);

		SPI_CS_PORT->BSRR = SPI_CS_PIN;
		int i = 0;
		for(i = 0; i < 200; i++);

}

void ADXL_setup(){
	SPI_write_reg(0x2d,0b00001000); //POWER_CTL
	SPI_write_reg(0x27,0b01110000); //ACT_INACT_CTL
	SPI_write_reg(0x24,30); //TRESH_ACT 62.5mg/lsb
	SPI_write_reg(0x2f,0b00000000);//INT_MAP
	SPI_write_reg(0x31,0b00000000);//DATA_FORMAT
	SPI_write_reg(0x2e,0b00010000);//INT_ENABLE
	SPI_read_reg(0x30);
}

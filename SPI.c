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


//void SPI_sendByte(uint8_t byteToSend)
//{
//	uint16_t timeout;
//	timeout = TIMEOUT_TIME;
//	while((((SPI_PORT->SR) & SPI_SR_TXE) == 0) & (timeout != 0))
//	{
//	timeout--;
//	}
//	SPI_PORT->DR = byteToSend;
//	 timeout = TIMEOUT_TIME;
//	while((((SPI_PORT->SR) & SPI_SR_TXE) == 0) & (timeout != 0))
//		{
//		timeout--;
//		}
//	while(((SPI_PORT->SR) & SPI_SR_BSY) != 0);
//}
//
//
//void SPI_writeData(uint8_t address, uint8_t dataToWrite)
//{
//    GPIO_LOW(SPI_CS_PORT, SPI_CS_PIN);
//
//    SPI_sendByte(address);
//    SPI_sendByte(dataToWrite);
//    GPIO_HIGH(SPI_CS_PORT, SPI_CS_PIN);
//}
//
//
//uint8_t SPI_readData(uint8_t address)
//{
//	address = address | 0x80;
//	GPIO_LOW(SPI_CS_PORT, SPI_CS_PIN);
//    SPI_sendByte(address);
//    uint8_t tempByte = SPI_sendByte(0x00);
//    GPIO_HIGH(SPI_CS_PORT, SPI_CS_PIN);
//    return tempByte;
//}

int16_t SPI_read_reg(char reg)
{
//	SPI_CS_PORT->BSRR = (SPI_CS_PIN << 16);
//
//	while(((SPI_PORT->SR) & SPI_SR_TXE) == 0);
////	SPI_PORT->SR &= ~SPI_SR_TXE;
//
//	SPI_PORT->DR = (reg | 0x80);
//
//	while(((SPI_PORT->SR) & SPI_SR_TXE) == 0);
////	SPI_PORT->SR &= ~SPI_SR_TXE;
//
//	while(((SPI_PORT->SR) & SPI_SR_RXNE) == 0);
////	SPI_PORT->SR &= ~SPI_SR_RXNE;
//
//
//	SPI_CS_PORT->BSRR = SPI_CS_PIN;
//
//	return SPI_PORT->DR;


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
	SPI_write_reg(0x24,20); //TRESH_ACT 62.5mg/lsb
	SPI_write_reg(0x2f,0b00000000);//INT_MAP
	SPI_write_reg(0x31,0b00000000);//DATA_FORMAT
	SPI_write_reg(0x2e,0b00010000);//INT_ENABLE
}










#ifdef LIS3DH_INTERFACE_SPI

void lis3dh_write_reg(char reg, char value)
{
	LIS3DH_SPI_CS_PORT->BSRR = 1 << (LIS3DH_SPI_CS_PIN + 16);

	while(((LIS3DH_SPI_PORT->SR) & SPI_SR_TXE) == 0);
	LIS3DH_SPI_PORT->DR = reg ;
	while(((LIS3DH_SPI_PORT->SR) & SPI_SR_TXE) == 0);
	LIS3DH_SPI_PORT->DR = value;

	/*int i = 0;
	for(; i < 20; i++);*/
	while(((LIS3DH_SPI_PORT->SR) & SPI_SR_BSY) != 0);
	//while(((LIS3DH_SPI_PORT->SR) & SPI_SR_TXE) == 0);

	LIS3DH_SPI_CS_PORT->BSRR = 1 << (LIS3DH_SPI_CS_PIN );
	int i = 0;
	for(i = 0; i < 200; i++);

}


int16_t lis3dh_read_reg(char reg)
{
	LIS3DH_SPI_CS_PORT->BSRR = 1 << (LIS3DH_SPI_CS_PIN + 16);

	while(((LIS3DH_SPI_PORT->SR) & SPI_SR_TXE) == 0);
	LIS3DH_SPI_PORT->DR = (reg | 0x80);
	while(((LIS3DH_SPI_PORT->SR) & SPI_SR_TXE) == 0);
	int x = LIS3DH_SPI_PORT->DR;

	LIS3DH_SPI_PORT->DR = 0;
	while(((LIS3DH_SPI_PORT->SR) & SPI_SR_RXNE) == 0);
	x = LIS3DH_SPI_PORT->DR;

	LIS3DH_SPI_PORT->DR = 0;
	while(((LIS3DH_SPI_PORT->SR) & SPI_SR_RXNE) == 0);

	LIS3DH_SPI_CS_PORT->BSRR = 1 << (LIS3DH_SPI_CS_PIN );
	int i = 0;
	for(; i < 200; i++);

	return LIS3DH_SPI_PORT->DR;
}











int lis3dh_get_status()
{
	return lis3dh_read_reg(0x27);
}


int lis3dh_get_interrupt_status()
{
	return lis3dh_read_reg(0x31);
}

int	lis3dh_get_x()
{
	int8_t x = lis3dh_read_reg(0x1E);
//	x |= (lis3dh_read_reg(0x29) << 8);
	return (int) x;
}

int	lis3dh_get_y()
{
	int8_t x = lis3dh_read_reg(0x1F);
//	x |= (lis3dh_read_reg(0x2B) << 8);
	return (int) x;
}

int	lis3dh_get_z()
{
	int8_t x = lis3dh_read_reg(0x20);
//	x |= (lis3dh_read_reg(0x2D) << 8);
	return (int) x;
}

/*void lis3dh_run()
{
	lis3dh_write_reg(0x20, 0x5F);
	lis3dh_write_reg(0x21, 0x0);
	lis3dh_write_reg(0x22, 0x0);
	lis3dh_write_reg(0x23, 0x0);
	lis3dh_write_reg(0x24, 0x0);
	lis3dh_write_reg(0x25, 0x0);
	lis3dh_write_reg(0x26, 0x0);
}
*/

void lis3dh_run(char frequency_range, char low_power_mode )
{
	lis3dh_write_reg(0x20, ((frequency_range & 0xF) << 4) | (low_power_mode?8 : 0 ) |7);
	lis3dh_write_reg(0x21, 0x0);
	lis3dh_write_reg(0x22, 0x0);
	lis3dh_write_reg(0x23, 0x0);
	lis3dh_write_reg(0x24, 0x0);
	lis3dh_write_reg(0x25, 0x0);
	lis3dh_write_reg(0x26, 0x0);
}

void lis3dh_stop()
{
	lis3dh_write_reg(0x20, 0);
}

int lis3dh_ok()
{
	if(lis3dh_read_reg(0xF) == 51) return 1;
	return 0;
}

void lis3dh_enable_click_interrupt(char treshold)
{
	lis3dh_write_reg(0x22, lis3dh_read_reg(0x22) | 0x80);
	lis3dh_write_reg(0x38, 0x15);
	lis3dh_write_reg(0x39, 0x57);
	lis3dh_write_reg(0x3A, treshold & 0x7f);
	lis3dh_write_reg(0x3B, 0x7F);

}

void lis3dh_enable_interrupt(char treshold, char cfg)
{
	lis3dh_write_reg(0x22, lis3dh_read_reg(0x22) | 0x40);
	lis3dh_write_reg(0x30, cfg);
	lis3dh_write_reg(0x32, treshold & 0x7f);
	lis3dh_write_reg(0x33, 0x0);

}



void lis3dh_disable_interrupt()
{
	lis3dh_write_reg(0x22, 0);

}



int lis3dh_get_average_x()
{
	int i = 0;
	int x = 0;
	for(; i < 4; i++)
	{
		int tmp;
		do {
			tmp = lis3dh_get_status();
		} while((tmp & 1) == 0) ;
		x += lis3dh_get_x();
	}
	return x >> 2;
}

int lis3dh_get_average_y()
{
	int i = 0;
	int x = 0;
	for(; i < 4; i++)
	{
		int tmp;
		do {
			tmp = lis3dh_get_status();
		} while((tmp & 2) == 0) ;
		x += lis3dh_get_y();
	}
	return x >> 2;
}

int lis3dh_get_average_z()
{
	int i = 0;
	int x = 0;
	for(; i < 4; i++)
	{
		int tmp;
		do {
			tmp = lis3dh_get_status();
		} while((tmp & 4) == 0) ;
		x += lis3dh_get_z();
	}
	return x >> 2;
}




char lis3dh_ismoving(){
	int x = lis3dh_get_x();
	int y = lis3dh_get_y();
	int z = lis3dh_get_z();
	int a  = ((x*x + z*z + y*y));
	if ((a > 3650) && (a < 4650)){
		return 0;
	}
	else
	{
		return 1;
	}

}


void lis3dh_get_average_all_unsign(int * rez)
{
	int i = 0;
	rez[0] = 0;
	rez[1] = 0;
	rez[2] = 0;
	for(; i < 4; i++)
	{
		int tmp;
		do {
			tmp = lis3dh_get_status();
		} while((tmp & 1) == 0) ;
		rez[0] += lis3dh_get_x();
		do {
			tmp = lis3dh_get_status();
		} while((tmp & 2) == 0) ;
		rez[1] += lis3dh_get_y();

		do {
			tmp = lis3dh_get_status();
		} while((tmp & 4) == 0) ;
		rez[2] += lis3dh_get_z();
	}
	for(i = 0; i < 3; i++)
	{
		rez[i] = abs(rez[i]);
	}
	rez[0] >>= 2;
	rez[1] >>= 2;
	rez[2] >>= 2;

}


void lis3dh_sort_data(int * a)
{
	int tmp;
	if(a[0] > a[1])
	{
		tmp = a[1];
		a[1] = a[0];
		a[0] = tmp;
	}
	if(a[1] > a[2])
	{
		tmp = a[1];
		a[1] = a[2];
		a[2] = tmp;
	}
	if(a[0] > a[1])
	{
		tmp = a[1];
		a[1] = a[0];
		a[0] = tmp;
	}
}



int lis3dh_calc_treshold()
{
	int a[3],  b[3];
	lis3dh_get_average_all_unsign(a);
	int i = 0;
	for(; i < 3; i++)
	{
		b[i] = a[i];
	}
	lis3dh_sort_data(a);
	int int_cfg = 0x40;
	if(a[2] - a[1] > ACC_ERROR * 2)
	{
		if(b[0] > a[1])
			int_cfg |= 0x10;
		else
			int_cfg |= 0x20;
		if(b[1] > a[1])
			int_cfg |= 0x4;
		else
			int_cfg |= 0x8;
		if(b[2] > a[1])
			int_cfg |= 0x1;
		else
			int_cfg |= 0x2;
		return (((a[1] + ACC_ERROR) )  | (int_cfg << 8));
	}
	else
	{
		int_cfg |= 0x2A;
		return (((a[2] + ACC_ERROR) ) | (int_cfg << 8));
	}

}
#endif

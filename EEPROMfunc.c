#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "defines.h"


 uint32_t * EEPROM_Address_32 = (uint32_t *)start_EEPROM;
 uint16_t * EEPROM_Address_16 = (uint16_t *)start_EEPROM;
 uint8_t * EEPROM_Address_8 = (uint8_t *)start_EEPROM;
 uint32_t EEPROMRead(uint32_t address, char bytes);

void EEPROMInit(){
	FLASH -> PEKEYR = FLASH_PEKEY1;
	FLASH -> PEKEYR = FLASH_PEKEY2;
}

void EEPROMEnable(){
if ((FLASH -> PECR & FLASH_PECR_PELOCK)){
	EEPROMInit();
}
	FLASH -> PECR |= FLASH_PECR_PROG;
}

void EEPROMDisable(){
	FLASH -> PECR &= ~FLASH_PECR_PROG;
}

void EEPROMWrite(int address,uint32_t data,char bytes){
EEPROMEnable();
	int temp1 = address / 4;
	temp1 = temp1 * 4;
	uint32_t temp = EEPROMRead(temp1,4);
	temp = ((temp&(~(0xFF<<(8*(address - temp1))))) | (data<<(8*(address - temp1))));
	*(uint32_t *)(start_EEPROM + temp1) = temp;

//	if (data == 0x00){
//		uint32_t temp = EEPROMRead(address,4);
//		temp = temp & 0xFFFFFF;
//		*(uint32_t *)(start_EEPROM + address) = temp;
//	}else{
//		if (bytes == 1) *(uint8_t *)(start_EEPROM + address) = data;
//		else if (bytes == 2) *(uint16_t *)(start_EEPROM + address) = data;
//		else *(uint32_t *)(start_EEPROM + address) = data;
//	}
	while (FLASH->SR & FLASH_SR_BSY);

EEPROMDisable();
}

uint32_t EEPROMRead(uint32_t address, char bytes) {
	if (bytes == 1) return *(uint8_t *)(start_EEPROM + address);
	else if (bytes == 2)return *(uint16_t *)(start_EEPROM + address);
	else return *(uint32_t *)(start_EEPROM + address);
}

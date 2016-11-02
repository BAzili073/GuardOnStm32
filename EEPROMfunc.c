#include "stm32l1xx_hal.h"
#include "stm32l151xba.h"
#include "defines.h"


 uint32_t * EEPROM_Address_32 = (uint32_t *)start_EEPROM;
 uint16_t * EEPROM_Address_16 = (uint16_t *)start_EEPROM;
 uint8_t * EEPROM_Address_8 = (uint8_t *)start_EEPROM;

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
	if (bytes == 1) *(uint8_t *)(start_EEPROM + address) = data;
	else if (bytes == 2) *(uint16_t *)(start_EEPROM + address) = data;
	else *(uint32_t *)(start_EEPROM + address) = data;
	while (FLASH->SR & FLASH_SR_BSY);

EEPROMDisable();
}

uint32_t EEPROMRead(uint32_t address, char bytes) {
	if (bytes == 1) return *(uint8_t *)(start_EEPROM + address);
	else if (bytes == 2)return *(uint16_t *)(start_EEPROM + address);
	else return *(uint32_t *)(start_EEPROM + address);
}

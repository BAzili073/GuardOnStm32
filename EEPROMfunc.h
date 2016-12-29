uint32_t *EEPROMAddress;

void EEPROMWrite(int address,uint32_t data,char bytes);
uint32_t EEPROMRead(uint32_t address, char bytes);
uint8_t EEPROMRead_id(uint32_t address);

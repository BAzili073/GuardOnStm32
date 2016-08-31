void one_wire_write_bit(uint8_t bit);
uint8_t one_wire_read_bit();
void one_wire_write_byte(uint8_t data);
char one_wire_send_presence();
uint8_t one_wire_read_bit();
void one_wire_write_byte(uint8_t data);
void one_wire_write_bit(uint8_t bit);
uint8_t one_wire_read_rom(uint8_t * buf);
uint8_t * one_wire_enum_next();
int16_t one_wire_read_temp_to_address(uint8_t address[8]);
uint8_t one_wire_start_conversion_temp();
uint8_t one_wire_check_keys();


char ds18x20_id[MAX_DS18x20][8];
char tm_id[MAX_TM][8];

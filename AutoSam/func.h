
#ifndef func_h
#define func_h
#include "header.h"

void findds();
float corrTemp(float);
void printAddress(DeviceAddress deviceAddress); // функция печати адреса DS18B20
float conc_f(float t);
float conc_s(float t);
float EEPROM_int_read(int addr);
float EEPROM_float_read(int addr);
//  void EEPROM_addr_read(int ad ); // на вход адрес eeprom и индекс датчика
void EEPROM_int_write(int addr, int num);
void EEPROM_float_write(int addr, float num);
void EEPROM_addr_write(int addr, byte *data); // на вход адрес eeprom и адрес датчика

#endif

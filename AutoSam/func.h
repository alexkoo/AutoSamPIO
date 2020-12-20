
#ifndef func_h
#define func_h
#include "header.h"

void findDS();
float corrTemp(float); // Коррекция температуры по давлению
void printAddress(DeviceAddress deviceAddress); // функция печати адреса DS18B20
float concFluid(float t); // Концентрация спирта в жидкости
float concSteam(float t); // Концентрация спирта в парах
float EEPROM_read(int addr, byte bytn); // чтение данных из EEPROM (адрес, количесство байтов(int=2, float=4))
//float EEPROM_int_read(int addr);
//float EEPROM_float_read(int addr);
//  void EEPROM_addr_read(int ad ); // на вход адрес eeprom и индекс датчика
void EEPROM_write(int addr, int num, byte bytn); // Запись данных в EEPROM (адрес, число, количесство байтов(int=2, float=4))
//void EEPROM_int_write(int addr, int num);
//void EEPROM_float_write(int addr, float num);
void EEPROM_addr_write(int addr, byte *data); // на вход адрес eeprom и адрес датчика

#endif

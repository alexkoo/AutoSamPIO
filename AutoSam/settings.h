#pragma once
#include "header.h"

// Настройки

const String VER = "1.7.1"; // Версия

// wifi
#ifndef STASSID
#define STASSID "krakozyabra"
#define STAPSK "10033val"
#endif
const char *ssid = STASSID;
const char *password = STAPSK;

#define HOSTNAME "autosam"

//**************************************************** EEPROM
const uint8_t autosam_mode_addr = 0;
const uint8_t flag_load_addr = 5;
const uint8_t press_corr_addr = 10;
const uint8_t min_hot_temp_addr = 20;
const uint8_t heating_rate_addr = 30;
const uint8_t max_tank_temp_addr = 40;
const uint8_t max_steam_temp_addr = 50;
const uint8_t steam_addr = 60;
const uint8_t pipe_addr = 70;
const uint8_t tank_addr = 80;
const uint8_t water_addr = 90;

const uint8_t flag_load = 17; // флаг первичной загрузки
uint8_t autosam_mode = 1;     // режим работы 1 ректификация 2 дистилляция 3 погода
uint8_t press_correction = 1; // коррекция давления
float min_hot_temp = 70.0;    // мин температура, при которой элемент считается горячим
float heating_rate = 5.5;     // заданная скорость нагрева, гр/мин
float max_tank_temp = 95.0;   // макс температура в кубе
float max_steam_temp = 90.0;  // макс температура  пара

//**************************************************************************************************// Управление

uint32_t ds_time_set = 3000;  // частота опроса DS
uint32_t bmx_time_set = 5000; // частота опроса BMX

//**************************************************************************************************// основные переменные

float steam_temp = -127;         // температура пара вверху колонны / сухопарник
float pipe_temp = -127;          // температура в царге на 2/3 высоты
float tank_temp = -127;          // температура в кубе
float water_temp = -127;         // температура охлаждающей воды или флегмы
float set_steam_temp_alc_st = 0; // установленная крепость

float steam_temp_alc_st = 0, pipe_temp_alc_st = 0, tank_temp_alc_st = 0; // Содержание спирта в парах
float steam_temp_alc_fl = 0, pipe_temp_alc_fl = 0, tank_temp_alc_fl = 0; // Содержание спирта в жидкости

GMedian3<float> SteamFilter; // указываем тип данных в <>
GMedian3<float> PipeFilter;  // 20-30mc
GMedian3<float> TankFilter;  // 6-11mc

float heating_rate_steam, heating_rate_pipe, heating_rate_tank; // скорость нагрева  гр/мин
RingAverage<float, 4> steam_rate;                               // фильтр скорости нагрева < тип, выборка >
RingAverage<float, 4> pipe_rate;                                // фильтр скорости нагрева < тип, выборка >
RingAverage<float, 4> tank_rate;                                // фильтр скорости нагрева < тип, выборка >

float atm_pressure = 754.0; // атмосферное давление текущее
float air_temp = 20.00;     // температура окружающего воздуха
bool BMP280_used = true;    // вспомогательная переменная при отсутствии датчика

//**************************************************************************************************//самогонный модуль
float set_temp_steam = 0, set_temp_pipe = 0; // уставка по температуре пара при достижении которой клапан отключается
uint32_t delay_steam = 30, delay_pipe = 30;  // временная задержка включения клапана по температуре (в секундах)
uint32_t valve_pause = 0;                    // минимальное время нахождения клапана в закрытом состоянии
bool valve_auto_mode = false;                // ручное управление клапаном

String auto_status = "990000"; //(status_process) (status_auto) (status_valve) (autosam_mode) предупреждения
uint8_t status_process = 0;    // 0 начало 1 бак нагревается 2  бак нагрет 3 нагрев узла 4 узел нагревается!! 5 узел нагрет 6 отбор хвостов 7 конец отбора
uint8_t status_auto = 0;       // 0 man 1auto, steam 2 auto, pipe
uint8_t status_valve = 0;      // 0 close 1 open

//**************************************************************************************************//Прочее
uint8_t debug = 1; // Редим отладки: 0 выкл 1-время выполнения

uint32_t debug_time_start, debug_time_stop, debug_time;
uint32_t free_mem;                      /// память
uint32_t timeloop_start, timeloop_stop; // отладка время выполнения

//

bool valve_invert = true; // true  NC false NO

#define ON HIGH // настройки для инвертирования клапапна
#define OFF LOW

// #define ON LOW  //настройки для инвертирования клапапна
// #define OFF HIGH
#ifndef settings_h
#define settings_h
#include "header.h"

//Настройки

//wifi
#ifndef STASSID
#define STASSID "krakozyabra"
#define STAPSK "10033val"
#endif
const char *ssid = STASSID;
const char *password = STAPSK;

//**************************************************************************************************//Версия
const String VER = "1.1.1"; // Версия

//**************************************************************************************************//EEPROM
const byte autosam_mode_addr = 0;
const byte press_corr_addr = 1;
const float min_hot_temp_addr =2;
const float heating_rate_addr = 6;
const float max_tank_temp_addr = 10;
const float max_steam_temp_addr = 14;

byte autosam_mode = 1;     //режим работы 1 ректификация 2 дистилляция 3 погода
byte press_corr = 1;     // коррекция давления
float min_hot_temp = 70.0; //мин температура, при которой элемент считается горячим
float heating_rate = 1;    // заданная скорость нагрева, гр/мин
float max_tank_temp = 95.0; // макс температура в кубе
float max_steam_temp = 80.0;  // макс температура  пара


// массив адресов датчиков
//DeviceAddress SteamSensor, PipeSensor, WaterSensor, TankSensor;
byte sens0[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte sens1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte sens2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
byte sens3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

byte SteamSensor[] = {0x28, 0xA8, 0x0A, 0x46, 0x92, 0x09, 0x02, 0xDE}; //Прописываем MAC адреса датчиков
byte PipeSensor[] = {0x28, 0x0F, 0x12, 0x43, 0x98, 0x18, 0x00, 0x3A};
byte TankSensor[] = {0x28, 0x35, 0x32, 0x46, 0x92, 0x09, 0x02, 0x9F};
byte WaterSensor[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//**************************************************************************************************// Управление

int lcd_max_num = 2;                //количество экранов от 0
unsigned long lcd_timer;           //таймер автопереключения экрана
unsigned long lcd_timer_set = 2000; //задержка автопереключения экрана, мс
int lcd_num = 0;               // переменная "экран", от которой зависит, какие показания будут выводиться на дисплей

int pressed = 0; // переменная "нажатие"

//**************************************************************************************************// основные переменные

float SteamTemp = -127;  // температура пара вверху колонны / сухопарник
float SteamTempNC = -127; // Некорректированная температура
float SteamTempVolS = 0;    // Содержание спирта в парах
float SteamTempVolF = 0;    // Содержание спирта в жидкости
float SteamTempO;        // предыдущая температура
bool SteamError = false;

float PipeTemp = -127;  // температура в царге на 2/3 высоты
float PipeTempNC = -127; // Некорректированная температура
float PipeTempVolS = 0;    // Содержание спирта в парах
float PipeTempVolF = 0;    // Содержание спирта в жидкости
float PipeTempO;        // предыдущая температура
bool PipeError = false;

float TankTemp = -127;  // температура в кубе
float TankTempNC = -127; // Некорректированная температура
float TankTempVolS = 0;    // Содержание спирта в парах
float TankTempVolF = 0;    // Содержание спирта в жидкости
float TankTempO;        // предыдущая температура
bool TankError = false;

float WaterTemp = -127; // температура охлаждающей воды или флегмы

float atm_pressure = 754.0;     // атмосферное давление текущее
float AirTemp = 20.00;     // температура окружающего воздуха

bool BMP280 = true;        // вспомогательная переменная при отсутствии датчика

unsigned long time_request; // таймер опроса датчиков


//**************************************************************************************************//самогонный модуль
float set_temp_steam = 0;        // уставка по температуре пара на 2/3 колонны, при достижении которой клапан отключается
float set_temp_pipe = 0;        // уставка по температуре пара вверху колонны, при достижении которой клапан отключается
unsigned long delay_steam = 30;           // временная задержка включения клапана по температуре на 2/3 колонны (в секундах)
unsigned long delay_pipe = 30;           // временная задержка включения клапана по температуре вверху колонны (в секундах)
unsigned long valve_pause; // минимальное время нахождения клапана в закрытом состоянии
bool valve_manual = false;         // ручное управление клапаном

String auto_status = " ";       // предупреждения
String warning = "не запущен"; // предупреждения
byte pcountsam;                // пауза (пред стадия)
byte countsam = 0;             // стадия процесса
float heating_rate_steam;                 // скорость нагрева сухопарника, гр/мин
float heating_rate_pipe;                 // скорость 2/3, гр/мин
float heating_rate_tank;                 // скорость нагрева бака, гр/мин
unsigned long heating_rate_int = 30000;    //интервал
unsigned long heating_rate_timer;            //таймер скорости изменения deltaT


//**************************************************************************************************//Прочее
byte debug = 2;          // Редим отладки: 0 выкл 1-основное  2-подробное 3 - фильтры
unsigned long debug_time; //период опроса

unsigned long free_mem;              ///память
unsigned long timeloop0, timeloop1; //отладка время выполнения

GMedian3<float> SteamFilter; // указываем тип данных в <>
GMedian3<float> PipeFilter;
GMedian3<float> TankFilter;
byte filter_enable = 1;

#endif

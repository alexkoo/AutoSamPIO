#pragma once

//**************************************************************************************************// LIB
//#include <Arduino.h>
//#include <ESP8266SSDP.h>      //esp8266 core убран в исходнике
//#include <DNSServer.h>        //esp8266 core  https://github.com/esp8266/Arduino
//#include <ESP8266mDNS.h>       //esp8266 core
//#include <WiFiUdp.h>           //esp8266 core
//#include <Wire.h>             //esp8266 core i2c
//#include <SPI.h>              //esp8266 core, нужен в platformio для BMx280I2C
//#include <ESP8266WiFi.h>      //https://github.com/esp8266/Arduino
//#include <FS.h>               //esp8266 core
//#include <WString.h> //esp8266 core
// #include <OneWire.h>           //https://github.com/PaulStoffregen/OneWire
// #include <DallasTemperature.h> //https://github.com/milesburton/Arduino-Temperature-Control-Library

#include <ESP8266WebServer.h> //esp8266 core

#include <EEPROM.h>           //esp8266 core
#include <LittleFS.h> // //esp8266 core 
#include <ArduinoOTA.h>        //esp8266 core
#include <WiFiManager.h>      //https://github.com/tzapu/WiFiManager
#include <LiquidCrystal_I2C.h> //https://github.com/marcoschwartz/LiquidCrystal_I2C.git
#include <BMx280I2C.h>         //https://bitbucket.org/christandlg/bmx280mi/
#include <GyverFilters.h>      //https://alexgyver.ru/gyverfilters/

#include <GyverDS18.h>      // https://github.com/GyverLibs/GyverDS18
#include <GyverDS18Array.h>     // https://github.com/GyverLibs/GyverDS18
//#include <microDS18B20.h> //https://github.com/GyverLibs/microDS18B20
//#define DS_CRC_USE_TABLE true  // Использовать таблицу для CRC. Быстрее, но +256 байт flash (<1мкс VS ~6мкс) (умолч. false)microDS18B20.h> microDS18B20.h>

#include <GyverNTP.h>
GyverNTP ntp(3);





//**************************************************************************************************// GPIO
#define buzzer_pin (16)         // (d0) Динамик GPIO 16  бел
// i2c scl (d1) син;
// i2c sda (d2) зел;
#define button (0)              // (d3, FLASH) GPIO0 кнопка (отпущена - HIGH, нажата - LOW)
#define ds_pin (2)             // (d4) GPIO 2   шина OneWire
#define valve_pin (14)              // (d5) GPIO 14  клапан отбора 
#define I2C_ADDRESS_BMx 0x76    //BMx280I2C
#define I2C_ADDRESS_LCD 0x27    //LCD
//**************************************************************************************************// INIT

#define ON HIGH  //настройки для инвертирования клапапна
#define OFF LOW

//#define ON LOW  //настройки для инвертирования клапапна
//#define OFF HIGH


#define DEBSTART if (debug==1) {debug_time_start = micros();}
#define DEBSTOP if (debug==1) {debug_time_stop = micros() - debug_time_start; debugTimePrint();} 


BMx280I2C bme(I2C_ADDRESS_BMx);         // с моим датчиком Adafruit_BMP280 работать не захотел
LiquidCrystal_I2C lcd(I2C_ADDRESS_LCD, 16, 2); // адрес дисплея на шине I2C, количество знаков, количество строк
ESP8266WebServer HTTP(80);          // Web интерфейс для устройства
File fsUploadFile;                  // Для файловой системы
WiFiServer telnetServer(23);
WiFiClient telnet;



/*
uint8_t sensor_address[][8] = {
    // массив адресов датчиков SteamSensor, PipeSensor, WaterSensor, TankSensor;
    {0x28, 0xA8, 0x0A, 0x46, 0x92, 0x09, 0x02, 0xDE},
    {0x28, 0x0F, 0x12, 0x43, 0x98, 0x18, 0x00, 0x3A},
    {0x28, 0xA8, 0x97, 0x46, 0x92, 0xF, 0x2, 0x2F},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};
MicroDS18B20<ds_pin, DS_ADDR_MODE, 4> ds_; // пин, (DS_ADDR_MODE) что будем работать с адресацией. Сам адрес передадим позже (в setAddress()), количество датчиков
*/

uint64_t ds_address[] = {     // массив адресов датчиков SteamSensor, PipeSensor, WaterSensor, TankSensor НОВЫЙ;
    0xDE020992460AA828,
    0x3A00189843120F28,
    0x2F020F924697A828,
    0x0000000000000000,
};
GyverDS18Array ds_sensors(ds_pin, ds_address, 4);
GyverDS18Single ds_single(ds_pin);  
bool setResolution(12);

// uint16_t getConversionTime();  // получить текущее время измерения температуры, мс


uint8_t steam_sensor_num = 0;
uint8_t pipe_sensor_num = 1;
uint8_t tank_sensor_num = 2;
uint8_t water_sensor_num = 3;






#include "settings.h"
#include "time_fs.h"
#include "http.h"
#include "func.h"
#include "logic.h"
#include "lcd.h"
#include "setup.h"
#include "loop.h"



//debug
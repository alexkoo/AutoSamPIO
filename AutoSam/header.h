#pragma once

//**************************************************************************************************// LIB
//#include <Arduino.h>
//#include <ESP8266WiFi.h>      //https://github.com/esp8266/Arduino
#include <WiFiManager.h>      //https://github.com/tzapu/WiFiManager
//#include <DNSServer.h>        //esp8266 core  https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h> //esp8266 core
//#include <ESP8266SSDP.h>      //esp8266 core убран в исходнике
//#include <FS.h>               //esp8266 core
#include <EEPROM.h>           //esp8266 core
//#include <Wire.h>             //esp8266 core i2c
//#include <SPI.h>              //esp8266 core, нужен в platformio для BMx280I2C
#include <LittleFS.h> // //esp8266 core 
// OTA
//#include <ESP8266mDNS.h>       //esp8266 core
//#include <WiFiUdp.h>           //esp8266 core
#include <ArduinoOTA.h>        //esp8266 core

#include <GyverFilters.h>      //https://alexgyver.ru/gyverfilters/
#include <LiquidCrystal_I2C.h> //https://github.com/marcoschwartz/LiquidCrystal_I2C.git
// #include <OneWire.h>           //https://github.com/PaulStoffregen/OneWire
// #include <DallasTemperature.h> //https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <BMx280I2C.h>         //https://bitbucket.org/christandlg/bmx280mi/

#include <GyverNTP.h>
GyverNTP ntp(3);

//**************************************************************************************************// GPIO
#define buzzer_pin (16)         // (d0) Динамик GPIO 16  бел
// i2c scl (d1) син;
// i2c sda (d2) зел;
#define button (0)              // (d3, FLASH) GPIO0 кнопка (отпущена - HIGH, нажата - LOW)
#define DS_PIN1 (2)             // (d4) GPIO 2   шина OneWire
//3v3;
//gnd;
#define valve (14)              // (d5) GPIO 14  клапан отбора 
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


#define DS_CRC_USE_TABLE true  // Использовать таблицу для CRC. Быстрее, но +256 байт flash (<1мкс VS ~6мкс) (умолч. false)microDS18B20.h> microDS18B20.h>
#include <microDS18B20.h> //https://github.com/GyverLibs/microDS18B20


#include "settings.h"
#include "time_fs.h"
#include "http.h"
#include "func.h"
#include "logic.h"
#include "lcd.h"
#include "setup.h"
#include "loop.h"




//debug
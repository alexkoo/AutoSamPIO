#ifndef header_h
#define header_h

//**************************************************************************************************// LIB
//#include <dummy.h>
//#include <Arduino.h>
#include <ESP8266WiFi.h>        //https://github.com/esp8266/Arduino
#include <WiFiManager.h>        //https://github.com/tzapu/WiFiManager
#include <DNSServer.h>          //esp8266 core  https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>   //esp8266 core
#include <ESP8266SSDP.h>        //esp8266 core 
#include <FS.h>                 //esp8266 core 
#include <EEPROM.h>             //esp8266 core 
#include <Wire.h>               //esp8266 core 
#include <SPI.h>                //esp8266 core, нужен в platformio для BMx280I2C
// OTA
#include <ESP8266mDNS.h>        //esp8266 core
#include <WiFiUdp.h>            //esp8266 core
#include <ArduinoOTA.h>         //esp8266 core
#include <GyverFilters.h>       //https://alexgyver.ru/gyverfilters/
#include <LiquidCrystal_I2C.h>  //https://github.com/marcoschwartz/LiquidCrystal_I2C.git
#include <OneWire.h>            //https://github.com/PaulStoffregen/OneWire
#include <DallasTemperature.h>  //https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <BMx280I2C.h>          //https://bitbucket.org/christandlg/bmx280mi/

//#include "LittleFS.h" // LittleFS is declared


//**************************************************************************************************// GPIO
#define ONE_WIRE_BUS 2 // GPIO 2 будет шиной OneWire 
#define I2C_ADDRESS 0x76 //BMx280I2C
#define valve (13) // GPIO 13 будет управлять клапаном отбора (если LOW - клапан включен, если HIGH - выключен)
#define button (0) // кнопка подключена к GPIO0 (отпущена - HIGH, нажата - LOW)
#define buz (16)   // Динамик

//**************************************************************************************************// INIT

BMx280I2C bme(I2C_ADDRESS); // с моим датчиком Adafruit_BMP280 работать не захотел
LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес дисплея на шине I2C, количество знаков, количество строк
ESP8266WebServer HTTP(80); // Web интерфейс для устройства
File fsUploadFile;         // Для файловой системы
WiFiServer telnetServer(23);
WiFiClient telnet;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


#include "SETTINGS.h"
#include "time_fs.h"
#include "http.h"
#include "lcd.h"
#include "logic.h"
#include "func.h"
#include "SETUP.h"
#include "LOOP.h"



#endif

#pragma once
#include "header.h"

void setup0()
{

  pinMode(valve_pin, OUTPUT); // назначаем выводу клапана функцию выхода
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(valve_pin, 0);
  digitalWrite(buzzer_pin, 0);

  // Wire.begin();
  Serial.begin(115200); // Настраиваем вывод отладки
  // delay(500);           // пауза небольшая
  Serial.print("AutoSam Version ");
  Serial.println(VER);

  lcd.begin(16, 2);       // задаём размерность дисплея
  lcd.init();             // инициализируем дисплей
  lcd.backlight();        // включаем подсветку
  lcd.setCursor(0, 0);    // устанавливаем курсор на вторую позицию в верхней строке
  lcd.print("AutoSam v"); // пишем AutoSam.
  lcd.print(VER);         // пишем версию ПО

  EEPROM.begin(512);
  EEPROM.get(autosam_mode_addr, autosam_mode);
  EEPROM.get(press_corr_addr, press_correction);
  EEPROM.get(min_hot_temp_addr, min_hot_temp);
  EEPROM.get(heating_rate_addr, heating_rate);
  EEPROM.get(max_tank_temp_addr, max_tank_temp);
  EEPROM.get(max_steam_temp_addr, max_steam_temp);

  EEPROM.get(steam_addr, ds_address[0]);
  EEPROM.get(pipe_addr, ds_address[1]);
  EEPROM.get(tank_addr, ds_address[2]);
  EEPROM.get(water_addr, ds_address[3]);


  
  

  WiFiManager wifiManager; // Включаем WiFiManager
  // Сначала модуль пытается подключиться к существующей сети. Если не удалось подключиться, (например, неизвестны SSID и пароль),
  // модуль запускается в режиме AP (точки доступа). Доступ к настройкам по адресу http://192.168.4.1
  wifiManager.autoConnect("AutoSamAP"); // задаём имя новой WiFi сети без пароля
  // wifiManager.autoConnect("AutoSamAP", "password"); // задаём имя новой WiFi сети c паролем (минимум 8 символов!)
  Serial.println("Connected! :)"); // если подключение произошло, сообщаем
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.print(WiFi.localIP());
  WiFi.hostname("HOSTNAME");

  delay(500);

  HTTP_Init(); // настраиваем HTTP интерфейс
  Serial.println("HTTP Ready, Starting UDP");
  ntp.begin(); // GyverNTP.h

  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Please connect Telnet Client, exit with ^] and 'quit'");

  ds_sensors.requestTemp();
  ds_sensors.setResolution(12);

  // sensors.setAddress((uint8_t *)sensor_address); // устанавливаем адреса DS18B20
  // sensors.setResolutionAll(12);                  // Установить разрешение 9-12 бит у всех датчиков на линии
  /*
                                                 // Время преобразования от точности
                                                 12 бит   | 750 мс 0.0625
                                                 11 бит   | 375 мс 0.125
                                                 10 бит   | 187 мс 0.25
                                                 9 бит    | 93 мс 0.5
*/

  if (!bme.begin()) // Инициализируем BMP280
  {
    telnet.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    BMP280_used = false;
  }
  else
  {
    telnet.println("Find a valid BMP280 sensor");
    BMP280_used = true;
  }
  if (BMP280_used == true)
  {
    bme.resetToDefaults();
    bme.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
    bme.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
  }

  Serial.println("OTA start");
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("autosamotg.local");

  // No authentication by default
  // ArduinoOTA.setPassword("1122");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    telnet.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    } });
  ArduinoOTA.begin();

  // /OTA

  lcd.clear(); // очищаем дисплей

  tone(buzzer_pin, 500, 100);

} // void setup

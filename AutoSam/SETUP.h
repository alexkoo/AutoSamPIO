#ifndef setup_h
#define setup_h
#include "header.h"

void initDebug()
{
  set_temp_steam = 78.5;
  set_temp_pipe = 78.6;
  valve_auto_mode = true;
}

void setup0()
{

  pinMode(valve, OUTPUT); // назначаем выводу клапана функцию выхода
  pinMode(buz, OUTPUT);
  digitalWrite(valve, 0);
  digitalWrite(buz, 0);
  // Wire.begin();
  Serial.begin(115200); // Настраиваем вывод отладки
  delay(500);           // пауза небольшая
  Serial.print("AutoSam Version ");
  Serial.println(VER);

  lcd.begin(16, 2);       // задаём размерность дисплея
  lcd.init();             // инициализируем дисплей
  lcd.backlight();        // включаем подсветку
  lcd.setCursor(0, 0);    // устанавливаем курсор на вторую позицию в верхней строке
  lcd.print("AutoSam v"); // пишем AutoSam.
  lcd.print(VER);         // пишем версию ПО

  EEPROM.begin(512);
  autosam_mode = EEPROM.read(autosam_mode_addr);
  press_corr = EEPROM.read(press_corr_addr);
  min_hot_temp = EEPROM_read(min_hot_temp_addr, 4);
  heating_rate = EEPROM_read(heating_rate_addr, 4);
  max_tank_temp = EEPROM_read(max_tank_temp_addr, 4);
  max_steam_temp = EEPROM_read(max_steam_temp_addr, 4);

  for (byte f = 0; f < 8; f++)
  {
    sens0[f] = EEPROM.read(18 + f);
    sens1[f] = EEPROM.read(26 + f);
    sens2[f] = EEPROM.read(34 + f);
    sens3[f] = EEPROM.read(42 + f);
  }
  WiFiManager wifiManager; //Включаем WiFiManager
  // Сначала модуль пытается подключиться к существующей сети.
  // Если не удалось подключиться, (например, неизвестны SSID и пароль),
  // модуль запускается в режиме AP (точки доступа).
  // Доступ к настройкам по адресу http://192.168.4.1

  wifiManager.autoConnect("AutoSamAP"); // задаём имя новой WiFi сети без пароля
  //wifiManager.autoConnect("AutoSamAP", "password"); // задаём имя новой WiFi сети c паролем (минимум 8 символов!)
  Serial.println("Connected! :)"); //если подключение произошло, сообщаем
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.print(WiFi.localIP());

  delay(500);

  HTTP_init(); //настраиваем HTTP интерфейс
  Serial.println("HTTP Ready, Starting UDP");
  udp.begin(localPort); // Инициализация UDP соединения с NTP сервером
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Please connect Telnet Client, exit with ^] and 'quit'");

  sensors.begin(); // стартуем датчики температуры
  // определяем устройства на шине
  Serial.print("Locating DS18B20...Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // Инициализируем датчики температуры по возрастанию серийных номеров
  /*
    if (!sensors.getAddress(sens0, 0))
    { Serial.println("Unable to find address for Device 0"); } // если адрес датчика 0 не найден
    if (!sensors.getAddress(sens1, 1))
    { Serial.println("Unable to find address for Device 1"); } // если адрес датчика 1 не найден
    if (!sensors.getAddress(sens2, 1))
    { Serial.println("Unable to find address for Device 2"); } // если адрес датчика 2 не найден
    if (!sensors.getAddress(sens3, 1))
    { Serial.println("Unable to find address for Device 3"); } // если адрес датчика 3 не найден
*/

  sensors.setResolution(SteamSensor, 12); // устанавливаем разрешение для датчика 0
  sensors.setResolution(PipeSensor, 12);  // устанавливаем разрешение для датчика 1
  sensors.setResolution(WaterSensor, 12); // устанавливаем разрешение для датчика 2
  sensors.setResolution(TankSensor, 12);  // устанавливаем разрешение для датчика 3

  Serial.print("SteamSensor Address: "); // пишем адрес и разрешение датчика 0
  printAddress(SteamSensor);
  Serial.print(" Resolution: ");
  Serial.println(sensors.getResolution(SteamSensor), DEC);

  Serial.print("PipeSensor Address: "); // пишем адрес и разрешение датчика 1
  printAddress(PipeSensor);
  Serial.print(" Resolution: ");
  Serial.println(sensors.getResolution(PipeSensor), DEC);

  Serial.print("WaterSensor Address: "); // пишем адрес и разрешение датчика 2
  printAddress(WaterSensor);
  Serial.print(" Resolution: ");
  Serial.println(sensors.getResolution(WaterSensor), DEC);

  Serial.print("TankSensor Address: "); // пишем адрес и разрешение датчика 3
  printAddress(TankSensor);
  Serial.print(" Resolution: ");
  Serial.println(sensors.getResolution(TankSensor), DEC);

  // Инициализируем BMP280

  Serial.println(F("BMP280 search..."));
  if (!bme.begin())
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    BMP280 = false;
  }
  else
  {
    Serial.println("Find a valid BMP280 sensor");
    BMP280 = true;
  }
  if (BMP280 == true)
  {
    bme.resetToDefaults();
    bme.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
    bme.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
  }

  Serial.println("OTA start");
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("esp8266_samogon");

  // No authentication by default
  // ArduinoOTA.setPassword("1122");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
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
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
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
    }
  });
  ArduinoOTA.begin();
  //  Serial.println("Ready");
  //  Serial.print("IP address: ");
  //  Serial.println(WiFi.localIP());
  // /OTA

  lcd.clear(); // очищаем дисплей

  tone(buz, 500, 100);

  if (debug == 1)
  {
    telnet.println("LOG: SteamTemp; PipeTemp; TankTemp; WaterTemp; air_temp; Pressure; Freemem; Timeloop");
  }

  if (debug == 4)
  {
    initDebug();
  }
} //void setup

#endif
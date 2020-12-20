#ifndef loop_h
#define loop_h
#include "header.h"

void loop0()
{ // вынос функции loop в отдельную вкладку
  free_mem = (ESP.getFreeHeap()); //свободная память

  if (debug == 2)
  {                                   // выполняется каждый цикл
    timeloop1 = millis() - timeloop0; // вывод время выполнения цикла
    timeloop0 = millis();
    telnet.print("Time Loop, ms: ");
    telnet.println(timeloop1);
  }

  ArduinoOTA.handle();
  HTTP.handleClient();
  delay(1);
  clok();

  if (telnetServer.hasClient())
  {
    if (!telnet || !telnet.connected())
    {
      if (telnet)
      {
        telnet.stop();
        Serial.println("Telnet Client Stop");
      }
      telnet = telnetServer.available();
      Serial.println("New Telnet client");
      telnet.print("Autosam telnet, debug mode: ");
      telnet.println(debug);
      telnet.flush(); // clear input buffer, else you get strange characters
    }
  }
  while (telnet.available())
  { // get data from Client
    Serial.write(telnet.read());
  }

  //********************************************
  // Обрабатываем нажатия кнопки переключения экранов дисплея

  if (digitalRead(button) == LOW && pressed == 0) //если кнопка нажата и переменная pressed равна 0 , то ...
  {
    pressed = 1;   //это нужно для того, чтобы с каждым нажатием кнопки происходило только одно действие плюс защита от "дребезга"
    lcd_num++;      // увеличиваем номер экрана на 1
    lcd.clear();   // при нажатии кнопки очищаем дисплей
    pcountsam = 0; // снимаем процесс с паузы
    if (lcd_num >= lcd_max_num)
      lcd_num = 0; // так как мы используем только одну кнопку, то переключать экраны будем циклично
  }
  if (digitalRead(button) == HIGH && pressed == 1)
    pressed = 0; //если кнопка НЕ нажата и pressed равна 1 ,то обнуляем pressed

  if (millis() - lcd_timer > lcd_timer_set)
  { // автопереключение экранов
    lcd_timer = millis();
    lcd_num++;
    lcd.clear();
    if (lcd_num > lcd_max_num)
      lcd_num = 0;
  }

  //*************************************************************************** // считываем температуры с датчиков

  if (millis() - time_request > 1000)
  { // период опроса датчиков, вычисления поправок
    time_request = millis();
    atm_pressure = bme.readPressure() * 0.00750063; // считываем атмосферное давление
    air_temp = bme.readTemperature();             // и температуру воздуха
    sensors.requestTemperatures();               // запрашиваем температуру у всех датчиков

    float TSteamTempN = sensors.getTempC(SteamSensor); // считываем температуру с датчика 0
    float TPipeTempN = sensors.getTempC(PipeSensor);   // считываем температуру с датчика 1
    float TWaterTemp = sensors.getTempC(WaterSensor);  // считываем температуру с датчика 2
    float TTankTempN = sensors.getTempC(TankSensor);   // считываем температуру с датчика 3

    if (TSteamTempN >= -30)
    { // проверка на ошибочные значения
      SteamError = false;
      SteamTempNC = TSteamTempN;
    }
    if (TPipeTempN >= -30)
    {
      PipeTempNC = TPipeTempN;
    }
    if (TWaterTemp >= -30)
    {
      WaterTemp = TWaterTemp;
    }
    if (TTankTempN >= -30)
    {
      TankTempNC = TTankTempN;
    }

    // поправки на давление и ручные
    SteamTemp = corrTemp(SteamTempNC) + 0.75; //  поправка. У меня  один из датчиков брешет
    PipeTemp = corrTemp(PipeTempNC) + 0.5;
    TankTemp = corrTemp(TankTempNC);

    // фильтрация
    if (filter_enable == 1)
    {
      SteamTemp = SteamFilter.filtered(SteamTemp);
      PipeTemp = PipeFilter.filtered(PipeTemp);
      TankTemp = TankFilter.filtered(TankTemp);
    }

    // вычисление крепости
    SteamTempVolS = concSteam(SteamTemp);
    SteamTempVolF = concFluid(SteamTemp);
    PipeTempVolS = concSteam(PipeTemp);
    PipeTempVolF = concFluid(PipeTemp);
    TankTempVolS = concSteam(TankTemp);
    TankTempVolF = concFluid(TankTemp);
  }

  // вычисление скорости отбора
  if (millis() - heating_rate_timer >= heating_rate_int)
  {                                                      // таймер heating_rate_timer сбрасывается каждые heating_rate_int миллисекунд
    heating_rate_timer = millis();                                     // перезаводится
    heating_rate_steam = (SteamTemp - SteamTempO) * (60000 / heating_rate_int); // heating_rate_steam - скорость нагрева град/мин
    if (heating_rate_steam > 40 || heating_rate_steam < -40)
    {
      heating_rate_steam = 0;
    }
    SteamTempO = SteamTemp;

    heating_rate_pipe = (PipeTemp - PipeTempO) * (60000 / heating_rate_int); // heating_rate_pipe - скорость нагрева град/мин
    if (heating_rate_pipe > 40 || heating_rate_pipe < -40)
    {
      heating_rate_pipe = 0;
    }
    PipeTempO = PipeTemp;

    heating_rate_tank = (TankTemp - TankTempO) * (60000 / heating_rate_int); // heating_rate_pipe - скорость нагрева град/мин
    if (heating_rate_pipe > 40 || heating_rate_pipe < -40)
    {
      heating_rate_pipe = 0;
    }
    TankTempO = TankTemp;
  }

  switch (autosam_mode)
  {
  case 1:
    rect(); // логика ректификации
    lcd1(); // вызываем функцию вывода на дисплей
    lcd_max_num = 2;
    break;
  case 2:
    samogon(); // логика самогонного аппарата
    lcd2();    // вызываем функцию вывода на дисплей
    lcd_max_num = 1;
    break;
  case 3:
    lcd3(); // вызываем функцию вывода на дисплей
    lcd_max_num = 0;
    break;
  }

  if (debug == 1)
  {
    if (millis() - debug_time >= 1000)
    {                       // выполняется раз в 1000 мс
      debug_time = millis(); // перезаводится

      // выводим температуры
      telnet.println(" ");
      telnet.println("LOG: ");
      telnet.print(SteamTemp);
      telnet.print("; ");
      telnet.print(SteamTemp);
      telnet.print("; ");
      telnet.print(PipeTemp);
      telnet.print("; ");
      telnet.print(TankTemp);
      telnet.print("; ");
      telnet.print(WaterTemp);
      telnet.print("; ");
      telnet.print(air_temp);
      telnet.print("; ");
      telnet.print(atm_pressure);
      telnet.print("; ");
      telnet.print(free_mem);
      telnet.print("; ");
      telnet.println(timeloop1);
      telnet.print(" Причина перезагрузки  ");
      telnet.println(ESP.getResetReason());

      findDS();
    }
  }

} // loop0

#endif
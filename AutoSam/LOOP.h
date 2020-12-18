#ifndef loop_h
#define loop_h

#include "header.h"

void loop0 () {   // вынос функции loop в отдельную вкладку
  //********************************************отладка

  freeMem = (ESP.getFreeHeap());  //свободная память

  if (debug == 2) { // выполняется каждый цикл
    timeloop1 = millis() - timeloop0; // вывод время выполнения цикла
    timeloop0 = millis();
    telnet.print ("Time Loop, ms: ");
    telnet.println(timeloop1);
  }

  ArduinoOTA.handle();
  HTTP.handleClient();
  delay(1);
  clok();

  if (telnetServer.hasClient()) {
    if (!telnet || !telnet.connected()) {
      if (telnet) {
        telnet.stop();
        Serial.println("Telnet Client Stop");
      }
      telnet = telnetServer.available();
      Serial.println("New Telnet client");
      telnet.print ("Autosam telnet, debug mode: ");
      telnet.println (debug);
      telnet.flush();  // clear input buffer, else you get strange characters
    }
  }
  while (telnet.available()) { // get data from Client
    Serial.write(telnet.read());
  }

  //********************************************
  // Обрабатываем нажатия кнопки переключения экранов дисплея

  if (digitalRead(button) == LOW && pressed == 0) //если кнопка нажата и переменная pressed равна 0 , то ...
  { pressed = 1; //это нужно для того, чтобы с каждым нажатием кнопки происходило только одно действие плюс защита от "дребезга"
    screen++;  // увеличиваем номер экрана на 1
    lcd.clear(); // при нажатии кнопки очищаем дисплей
    pcountsam = 0; // снимаем процесс с паузы
    if (screen >= lcd_n) screen = 0; // так как мы используем только одну кнопку, то переключать экраны будем циклично
  }
  if (digitalRead(button) == HIGH && pressed == 1) pressed = 0; //если кнопка НЕ нажата и pressed равна 1 ,то обнуляем pressed

  if (millis() - tims > timescr) { // автопереключение экранов
    tims = millis();
    screen++; lcd.clear(); if (screen > lcd_n ) screen = 0;
  }

  //*************************************************************************** // считываем температуры с датчиков



  if (millis() - timedat > 1000) {                  // период опроса датчиков, вычисления поправок
    timedat = millis();
    Apressure = bme.readPressure() * 0.00750063;    // считываем атмосферное давление
    AirTemp = bme.readTemperature();               // и температуру воздуха
    sensors.requestTemperatures();                // запрашиваем температуру у всех датчиков

    float TSteamTempN = sensors.getTempC(SteamSensor); // считываем температуру с датчика 0
    float TPipeTempN = sensors.getTempC(PipeSensor);   // считываем температуру с датчика 1
    float TWaterTemp = sensors.getTempC(WaterSensor);    // считываем температуру с датчика 2
    float TTankTempN = sensors.getTempC(TankSensor);      // считываем температуру с датчика 3

    if  (TSteamTempN >= -30) { // проверка на ошибочные значения
      SteamError = false;
      SteamTempN = TSteamTempN;
    }
    if  (TPipeTempN >= -30) {
      PipeTempN = TPipeTempN;
    }
    if  (TWaterTemp >= -30) {
      WaterTemp = TWaterTemp;
    }
    if  (TTankTempN >= -30) {
      TankTempN = TTankTempN;
    }

    // поправки на давление и ручные
    SteamTemp = corrTemp(SteamTempN) +0.75;//  поправка. У меня  один из датчиков брешет
    PipeTemp = corrTemp(PipeTempN )  +0.5;
    TankTemp = corrTemp(TankTempN);



    // фильтрация
    if (filter_enable ==1 ){
    SteamTemp =  SteamFilter.filtered(SteamTemp);
    PipeTemp =  PipeFilter.filtered(PipeTemp);
    TankTemp =  TankFilter.filtered(TankTemp);
    }
    
    // вычисление крепости
    SteamTempS = conc_s (SteamTemp);
    SteamTempF = conc_f (SteamTemp);
    PipeTempS  = conc_s (PipeTemp);
    PipeTempF  = conc_f (PipeTemp);
    TankTempS  = conc_s (TankTemp);
    TankTempF  = conc_f (TankTemp);

  }

  // вычисление скорости отбора
  if (millis() - dtim >= dint) { // таймер dtim сбрасывается каждые dint миллисекунд
    dtim = millis(); // перезаводится
    deltaTs = (SteamTemp - SteamTempO) * (60000 / dint); // deltaTs - скорость нагрева град/мин
    if (deltaTs > 40 || deltaTs < -40 ) {
      deltaTs = 0;
    }
    SteamTempO = SteamTemp;

    deltaTp = (PipeTemp - PipeTempO) * (60000 / dint); // deltaTp - скорость нагрева град/мин
    if (deltaTp > 40 || deltaTp < -40 ) {
      deltaTp = 0;
    }
    PipeTempO = PipeTemp;

    deltaTt = (TankTemp - TankTempO) * (60000 / dint); // deltaTp - скорость нагрева град/мин
    if (deltaTp > 40 || deltaTp < -40 ) {
      deltaTp = 0;
    }
    TankTempO = TankTemp;
  }



  switch (modesam) {
    case 1:
      rect (); // логика ректификации
      lsd1(); // вызываем функцию вывода на дисплей
      lcd_n = 2;
      break;
    case 2:
      samogon(); // логика самогонного аппарата
      lsd2(); // вызываем функцию вывода на дисплей
      lcd_n = 1;
      break;
    case 3:
      lsd3(); // вызываем функцию вывода на дисплей
      lcd_n = 0;
      break;
  }



  if (debug ==1 ) {
    if (millis() - debugTime >= 1000) { // выполняется раз в 1000 мс
      debugTime = millis(); // перезаводится

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
      telnet.print(AirTemp);
      telnet.print("; ");
      telnet.print(Apressure);
      telnet.print("; ");
      telnet.print(freeMem);
      telnet.print("; ");
      telnet.println(timeloop1);
    telnet.print ( " Причина перезагрузки  ");
    telnet.println (ESP.getResetReason());
    
 findds();
    
    }
  }

} // loop0

#endif
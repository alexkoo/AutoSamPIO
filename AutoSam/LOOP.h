#pragma once
#include "header.h"

void loop0() // вынос функции loop в отдельную вкладку
{
  if (debug >= 1)
    timeloop_start = micros();
  free_mem = (ESP.getFreeHeap()); // свободная память

  ArduinoOTA.handle();
  HTTP.handleClient();
  ntp.tick();   // guiverNTP
  telnetLoop(); // обработчик telnet
  lcdChange();  // таймер переключения экранов

  yield(); // прервание для работы wifi

  //*************************************************************************** // считываем температуры с датчиков
  static uint32_t bmx_timer; // таймер опроса датчика давления
  if (millis() - bmx_timer > bmx_time_set)
  {
    bmx_timer = millis();
    atm_pressure = bme.readPressure() * 0.00750063; // считываем атмосферное давление  104ms
    air_temp = bme.readTemperature();               // и температуру воздуха 104ms
    air_temp = round(air_temp * 10) / 10;           // округляем до 0,1
  }
    yield(); // прервание для работы wifi





    static uint32_t ds_timer; // таймер опроса датчиков
    if (millis() - ds_timer > ds_time_set)
    
    //if (ds_sensors.ready()) {  // измерения готовы по таймеру новая функция
    {
      ds_timer = millis();
    
/*
      //DEBSTART
      //DEBSTOP

      yield(); // прервание для работы wifi
      float steam_temp_nc = ds_sensors.getTemp(steam_sensor_num); // считываем с каждого датчика  13ms со всех 50ms
      yield();                                                 // прервание для работы wifi
      float pipe_temp_nc = ds_sensors.getTemp(pipe_sensor_num);
      yield(); // прервание для работы wifi
      float tank_temp_nc = ds_sensors.getTemp(tank_sensor_num);
      yield(); // прервание для работы wifi
      float water_temp = ds_sensors.getTemp(water_sensor_num);
      yield(); // прервание для работы wifi

      */
     yield(); // прервание для работы wifi
     float steam_temp_nc = ds_sensors.readTemp(steam_sensor_num); // считываем с каждого датчика  13ms со всех 50ms
     yield();                                                 // прервание для работы wifi
     float pipe_temp_nc = ds_sensors.readTemp(pipe_sensor_num);
     yield(); // прервание для работы wifi
     float tank_temp_nc = ds_sensors.readTemp(tank_sensor_num);
     yield(); // прервание для работы wifi
     float water_temp = ds_sensors.readTemp(water_sensor_num);
     yield(); // прервание для работы wifi


      ds_sensors.requestTemp(); // запрашиваем новые температуры 26-1200ms
  yield(); // прервание для работы wifi







      float steam_temp_f = SteamFilter.filtered(steam_temp_nc);
      float tank_temp_f = TankFilter.filtered(tank_temp_nc); //
      float pipe_temp_f = PipeFilter.filtered(pipe_temp_nc); //

      // поправки на давление и ручные 1-2mc
      steam_temp = corrTemp(steam_temp_f); //  поправка. У меня  один из датчиков брешет
      pipe_temp = corrTemp(pipe_temp_f);
      tank_temp = corrTemp(tank_temp_f); //

      // вычисление крепости 0-2000мс
      steam_temp_alc_st = concSteam(steam_temp); // 0,1mc
      steam_temp_alc_fl = concFluid(steam_temp); // 70-90mc 0,1мс
      yield();                                   // прервание для работы wifi
      pipe_temp_alc_st = concSteam(pipe_temp);
      pipe_temp_alc_fl = concFluid(pipe_temp);
      yield(); // прервание для работы wifi
      tank_temp_alc_st = concSteam(tank_temp);
      tank_temp_alc_fl = concFluid(tank_temp);
      set_steam_temp_alc_st = concSteam(set_temp_steam);
      //}

      // вычисление скорости отбора

      static uint32_t heating_rate_timer;                // таймер скорости изменения deltaT
      static float steam_temp_prev, pipe_temp_prev, tank_temp_prev; // предыдущая температура

      if (millis() - heating_rate_timer >= heating_rate_int * 1000)
      {
        heating_rate_timer = millis();
        
        heating_rate_steam = (steam_temp - steam_temp_prev) * (60 / heating_rate_int);
        heating_rate_steam = steam_rate.filtered(heating_rate_steam);
        steam_temp_prev = steam_temp;

        heating_rate_pipe = (pipe_temp - pipe_temp_prev) * (60 / heating_rate_int);
        heating_rate_pipe = pipe_rate.filtered(heating_rate_pipe);
        pipe_temp_prev = pipe_temp;


        heating_rate_tank = (tank_temp - tank_temp_prev) * (60 / heating_rate_int);
        heating_rate_tank = tank_rate.filtered(heating_rate_tank);
        tank_temp_prev = tank_temp;

      }
    }

    yield(); // прервание для работы wifi

    switch (autosam_mode)
    {
    case 1:
      rectification(); // логика ректификации
      lcdRect();       // вызываем функцию вывода на дисплей 60mc
      lcd_max_num = 2;

      break;
    case 2:
      samogon(); // логика самогонного аппарата
      lcdSam();  // вызываем функцию вывода на дисплей
      lcd_max_num = 1;
      break;
    case 3:
      lcdTerm(); // вызываем функцию вывода на дисплей
      lcd_max_num = 0;
      break;
    }

    if (debug >= 1)
      timeloop_stop = micros() - timeloop_start;

  } // loop0

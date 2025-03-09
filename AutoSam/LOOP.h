#pragma once
#include "header.h"

void loop0()
{

  if (debug >= 1)
    timeloop_start = micros();
  // free_mem = (ESP.getFreeHeap()); // свободная память

  ArduinoOTA.handle();
  HTTP.handleClient();
  ntp.tick();   // guiverNTP
  telnetLoop(); // обработчик telnet
  lcdChange();  // таймер переключения экранов

  yield(); // прервание для работы wifi

  //*************************************************************************** // считываем BMX
  static uint32_t bmx_timer; // таймер опроса датчика давления
  if (millis() - bmx_timer > bmx_time_set)
  {
    bmx_timer = millis();
    atm_pressure = bme.readPressure() * 0.00750063; // считываем атмосферное давление  104ms
    air_temp = bme.readTemperature();               // и температуру воздуха 104ms
    air_temp = round(air_temp * 10) / 10;           // округляем до 0,1
  }
  yield(); // прервание для работы wifi

  //*************************************************************************** // считываем DS
  uint32_t ds_timer = 0; // таймер опроса датчиков
  uint32_t ds_time = (millis() - ds_timer);
  if (ds_time > ds_time_set)
  { //   DS18 timer

    ds_timer = millis();

    float steam_temp_nc, pipe_temp_nc, tank_temp_nc, water_temp_nc; // температуры сырые
    yield();                                                        // прервание для работы wifi
    if (ds_sensors.readTemp(steam_sensor_num))
      steam_temp_nc = ds_sensors.getTemp(); // считываем темппературу с датчикака     ; // получаем считанную температуру //13ms со всех 50ms//
    else
      steam_temp_nc = -127;
    yield();
    if (ds_sensors.readTemp(pipe_sensor_num))
      pipe_temp_nc = ds_sensors.getTemp();
    else
      pipe_temp_nc = -127;
    yield(); // прервание для работы wifi
    if (ds_sensors.readTemp(tank_sensor_num))
      tank_temp_nc = ds_sensors.getTemp();
    else
      tank_temp_nc = -127;
    yield(); // прервание для работы wifi
    if (ds_sensors.readTemp(water_sensor_num))
      water_temp_nc = ds_sensors.getTemp();
    else
      water_temp_nc = -127;
    yield(); // прервание для работы wifi

    ds_sensors.requestTemp(); // запрашиваем новые температуры 26-1200ms
    yield();                  // прервание для работы wifi

    float steam_temp_f = SteamFilter.filtered(steam_temp_nc);
    float tank_temp_f = TankFilter.filtered(tank_temp_nc); //
    float pipe_temp_f = PipeFilter.filtered(pipe_temp_nc); //
    water_temp = round(water_temp_nc * 10) / 10;           // округление до 0.1

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

    //*************************************************************************** // Скорость отбора

    uint32_t heating_rate_timer = 0;                                                            // таймер скорости изменения deltaT
    float steam_temp_prev = steam_temp, pipe_temp_prev = pipe_temp, tank_temp_prev = tank_temp; // предыдущая температура

    if (millis() - heating_rate_timer >= ds_time)
    {
      heating_rate_timer = millis();

      heating_rate_steam = (steam_temp - steam_temp_prev) * (60 / ds_time);
      heating_rate_steam = steam_rate.filtered(heating_rate_steam);
      steam_temp_prev = steam_temp;

      heating_rate_pipe = (pipe_temp - pipe_temp_prev) * (60 / ds_time);
      heating_rate_pipe = pipe_rate.filtered(heating_rate_pipe);
      pipe_temp_prev = pipe_temp;

      heating_rate_tank = (tank_temp - tank_temp_prev) * (60 / ds_time);
      heating_rate_tank = tank_rate.filtered(heating_rate_tank);
      tank_temp_prev = tank_temp;
    }
  }

  yield(); // прервание для работы wifi

  switch (autosam_mode)
  {
  case 1:
    rectification(); // логика ректификации
    temp_status();
    lcdRect(); // вызываем функцию вывода на дисплей 60mc
    lcd_max_num = 2;
    break;
  case 2:
    temp_status(); // логика самогонного аппарата
    lcdSam();      // вызываем функцию вывода на дисплей
    lcd_max_num = 1;
    break;
  case 3:
    lcdTerm(); // вызываем функцию вывода на дисплей
    lcd_max_num = 0;
    break;
  }

  auto_status = 99;
  auto_status += status_process;
  auto_status += status_auto;
  auto_status += status_valve;
  auto_status += autosam_mode;
  // telnet.println(auto_status);

  if (debug >= 1)
    timeloop_stop = micros() - timeloop_start;

} // loop0

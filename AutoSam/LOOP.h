#pragma once
#include "header.h"
uint32_t ds_timer = 0; // таймер опроса датчиков
uint32_t heating_rate_timer = 0;                                                            // таймер скорости изменения deltaT

float steam_temp_prev = steam_temp, pipe_temp_prev = pipe_temp, tank_temp_prev = tank_temp; // предыдущая температура
void loop0()
{

  if (debug >= 1)
    timeloop_start = micros();
  // free_mem = (ESP.getFreeHeap()); // свободная память

  ArduinoOTA.handle();
  HTTP.handleClient();

#ifdef NodeMCU
  MDNS.update();
#endif

  ntp.tick();   // guiverNTP
  telnetLoop(); // обработчик telnet
  lcdChange();  // таймер переключения экранов

  yield(); // прервание для работы wifi

  //*************************************************************************** // считываем BMX
  static uint32_t bmx_timer; // таймер опроса датчика давления
  if (millis() - bmx_timer > bmx_time_set)
  {
    bmx_timer = millis();
    // while (bme.isMeasuring())
    air_temp = bme.readTemperature();     // и температуру воздуха 
    air_temp = round(air_temp * 10) / 10; // округляем до 0,1
    atm_pressure = pressureToMmHg(bme.readPressure());
    bme.oneMeasurement();
  }
  yield(); // прервание для работы wifi

  //*************************************************************************** // считываем DS
  
  uint32_t ds_time = (millis() - ds_timer); //период опроса DS
  if (ds_time > ds_time_set)
  { //   DS18 timer

    ds_timer = millis();

    float steam_temp_nc, pipe_temp_nc, tank_temp_nc, water_temp_nc; // температуры сырые
    yield();                                                        // прервание для работы wifi
    if (dsSensors.readTemp(STEAM_SENSOR_NUM))
      steam_temp_nc = dsSensors.getTemp(); // считываем темппературу с датчикака     ; // получаем считанную температуру //13ms со всех 50ms//
    else
      steam_temp_nc = -127;
    yield();
    if (dsSensors.readTemp(PIPE_SENSOR_NUM))
      pipe_temp_nc = dsSensors.getTemp();
    else
      pipe_temp_nc = -127;
    yield(); // прервание для работы wifi
    if (dsSensors.readTemp(TANK_SENSOR_NUM))
      tank_temp_nc = dsSensors.getTemp();
    else
      tank_temp_nc = -127;
    yield(); // прервание для работы wifi
    if (dsSensors.readTemp(WATER_SENSOR_NUM))
      water_temp_nc = dsSensors.getTemp();
    else
      water_temp_nc = -127;
    yield(); // прервание для работы wifi

    dsSensors.requestTemp(); // запрашиваем новые температуры 26-1200ms
    yield();                 // прервание для работы wifi

    float steam_temp_f = SteamFilter.filtered(steam_temp_nc);
    float tank_temp_f = TankFilter.filtered(tank_temp_nc); //
    float pipe_temp_f = PipeFilter.filtered(pipe_temp_nc); //
    water_temp = round(water_temp_nc * 10) / 10;           // округление до 0.1

    // поправки на давление и ручные 1-2mc
    steam_temp = corrTemp(steam_temp_f) + steam_corr; //  поправка. У меня  один из датчиков брешет
    pipe_temp = corrTemp(pipe_temp_f) + pipe_corr;
    tank_temp = corrTemp(tank_temp_f) + tank_corr; //

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

  

    //if (millis() - heating_rate_timer >= ds_time)
    //{
      

     // heating_rate_timer = millis();

      telnet.print(" rate 2");
     /// telnet.print(" heating_rate_timer ");
     // telnet.print(heating_rate_timer);
      telnet.print(" ds_time ");
      telnet.print(ds_time);
      telnet.print("mint");
      telnet.print(min_hot_temp);
      telnet.println();


      heating_rate_steam = (steam_temp - steam_temp_prev) * (60000 / ds_time);
      heating_rate_steam = steam_rate.filtered(heating_rate_steam);
      steam_temp_prev = steam_temp;

      heating_rate_pipe = (pipe_temp - pipe_temp_prev) * (60000 / ds_time);
      heating_rate_pipe = pipe_rate.filtered(heating_rate_pipe);
      pipe_temp_prev = pipe_temp;

      heating_rate_tank = (tank_temp - tank_temp_prev) * (60000 / ds_time);
      telnet.print(heating_rate_tank);
      heating_rate_tank = tank_rate.filtered(heating_rate_tank);
      telnet.print(heating_rate_tank);
      tank_temp_prev = tank_temp;
      telnet.println();
   // }
    
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

#ifndef loop_h
#define loop_h
#include "header.h"

void loop0() // вынос функции loop в отдельную вкладку
{
  DEBSTOP
  DEBSTART

  free_mem = (ESP.getFreeHeap()); //свободная память

  ArduinoOTA.handle();
  HTTP.handleClient();

  ntp.tick(); // guiverNTP
  // delay(1);

  telnetLoop();

  if (millis() - lcd_timer > lcd_timer_set)
  { // автопереключение экранов
    lcd_timer = millis();
    lcd_num++;
    lcd.clear();
    if (lcd_num > lcd_max_num)
      lcd_num = 0;
  }

  //*************************************************************************** // считываем температуры с датчиков
  if (millis() - bmx_timer > bmx_time_set)
  {
    bmx_timer = millis();
    atm_pressure = bme.readPressure() * 0.00750063; // считываем атмосферное давление  104ms
    air_temp = bme.readTemperature();               // и температуру воздуха 104ms
  }

  // конструкция программного таймера на 1c
  static uint32_t tmr;
  if (millis() - tmr >= 1000)
  {
    tmr = millis();

    float SteamTempN = sensors.getTemp(steam_sensor_num); // считываем с каждого датчика  13ms со всех 50ms
    float PipeTempN = sensors.getTemp(pipe_sensor_num);
    float TankTempN = sensors.getTemp(tank_sensor_num);
    float WaterTemp = sensors.getTemp(water_sensor_num);

    // запрашиваем новые
    sensors.requestTempAll();

  
    /*
      if (millis() - ds_timer > ds_time_set)
      { // период опроса датчиков, вычисления поправок
        sensors.requestTemperatures();                    // запрашиваем температуру у всех датчиков 14ms
        ds_timer = millis();

    sensors.requestTemperatures();                    // запрашиваем температуру у всех датчиков 14ms
    float steam_temp_nc = sensors.getTempC(SteamSensor); // считываем с каждого датчика  13ms со всех 50ms
    float pipe_temp_nc = sensors.getTempC(PipeSensor);
    float water_temp = sensors.getTempC(WaterSensor);
    float tank_temp_nc = sensors.getTempC(TankSensor);

    float steam_temp_filtered = SteamFilter.filtered(steam_temp_nc);
    float tank_temp_filtered = TankFilter.filtered(tank_temp_nc); //
    float pipe_temp_fitered = PipeFilter.filtered(pipe_temp_nc); //

    // поправки на давление и ручные 1-2mc
    steam_temp = corrTemp(steam_temp_filtered) + 0.5; //  поправка. У меня  один из датчиков брешет
    pipe_temp = corrTemp(pipe_temp_fitered) + 0.5;
    tank_temp = corrTemp(tank_temp_filtered); //

    // вычисление крепости 0-2000мс
    steam_temp_alc_st = concSteam(steam_temp); // 0,1mc
    steam_temp_alc_fl = concFluid(steam_temp); // 70-90mc 0,1мс
    pipe_temp_alc_st = concSteam(pipe_temp);
    pipe_temp_alc_fl = concFluid(pipe_temp);
    tank_temp_alc_st = concSteam(tank_temp);
    tank_temp_alc_fl = concFluid(tank_temp);
    set_steam_temp_alc_st = concSteam(set_temp_steam);
  }

  // вычисление скорости отбора
  if (millis() - heating_rate_timer >= heating_rate_int)
  {                                                                             // таймер heating_rate_timer сбрасывается каждые heating_rate_int миллисекунд
    heating_rate_timer = millis();                                              // перезаводится
    heating_rate_steam = (steam_temp - steam_temp_prev) * (60000 / heating_rate_int); // heating_rate_steam - скорость нагрева град/мин
    if (heating_rate_steam > 40 || heating_rate_steam < -40)
    {
      heating_rate_steam = 0;
    }
    steam_temp_prev = steam_temp;

    heating_rate_pipe = (pipe_temp - pipe_temp_prev) * (60000 / heating_rate_int); // heating_rate_pipe - скорость нагрева град/мин
    if (heating_rate_pipe > 40 || heating_rate_pipe < -40)
    {
      heating_rate_pipe = 0;
    }
    pipe_temp_prev = pipe_temp;

    heating_rate_tank = (tank_temp - tank_temp_prev) * (60000 / heating_rate_int); // heating_rate_pipe - скорость нагрева град/мин
    if (heating_rate_pipe > 40 || heating_rate_pipe < -40)
    {
      heating_rate_pipe = 0;
    }
    tank_temp_prev = tank_temp;
  }

  switch (autosam_mode)
  {
  case 1:
    rectification(); // логика ректификации
    lcd1(); // вызываем функцию вывода на дисплей 60mc
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

} // loop0

#endif
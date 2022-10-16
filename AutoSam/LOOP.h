#ifndef loop_h
#define loop_h
#include "header.h"

void loop0()// вынос функции loop в отдельную вкладку
{    
  DEBSTOP
  DEBSTART
                               
  free_mem = (ESP.getFreeHeap()); //свободная память

  ArduinoOTA.handle();
  HTTP.handleClient();

   ntp.tick(); //guiverNTP
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

  if (millis() - ds_timer > ds_time_set)
  { // период опроса датчиков, вычисления поправок
    sensors.requestTemperatures();                    // запрашиваем температуру у всех датчиков 14ms
    ds_timer = millis();

  
    float SteamTempN = sensors.getTempC(SteamSensor); // считываем с каждого датчика  13ms со всех 50ms
    float PipeTempN = sensors.getTempC(PipeSensor);
    float WaterTemp = sensors.getTempC(WaterSensor);
    float TankTempN = sensors.getTempC(TankSensor);

    float SteamTempF = SteamFilter.filtered(SteamTempN);
    float TankTempF = TankFilter.filtered(TankTempN); //
    float PipeTempF = PipeFilter.filtered(PipeTempN); //

    // поправки на давление и ручные 1-2mc
    SteamTemp = corrTemp(SteamTempF) + 0.5; //  поправка. У меня  один из датчиков брешет
    PipeTemp = corrTemp(PipeTempF) + 0.5;
    TankTemp = corrTemp(TankTempF); //

    // вычисление крепости 0-2000мс
    SteamTempVolS = concSteam(SteamTemp); // 0,1mc
    SteamTempVolF = concFluid(SteamTemp); // 70-90mc 0,1мс
    PipeTempVolS = concSteam(PipeTemp);
    PipeTempVolF = concFluid(PipeTemp);
    TankTempVolS = concSteam(TankTemp);
    TankTempVolF = concFluid(TankTemp);
    SetSteamTempVolS = concSteam(set_temp_steam);
  }

  // вычисление скорости отбора
  if (millis() - heating_rate_timer >= heating_rate_int)
  {                                                                             // таймер heating_rate_timer сбрасывается каждые heating_rate_int миллисекунд
    heating_rate_timer = millis();                                              // перезаводится
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

#pragma once
#include "header.h"

String millis2time() // функция формирования строки "время работы модуля"
{
  String Time = ""; // начинаем с пустой строки
  unsigned long ss;
  byte mm, hh;
  ss = millis() / 1000;            // вычисляем количество секунд с начала работы модуля
  hh = ss / 3600;                  // вычисляем количество часов
  mm = (ss - hh * 3600) / 60;      // вычисляем количество минут
  ss = (ss - hh * 3600) - mm * 60; // вычисляем количество секунд
  if (hh < 10)
    Time += "0";            // добавляем незначащий ноль к часам
  Time += (String)hh + ":"; // добавляем двоеточие между часами и минутами
  if (mm < 10)
    Time += "0";            // добавляем незначащий ноль к минутам
  Time += (String)mm + ":"; // добавляем двоеточие между минутами и секундами
  if (ss < 10)
    Time += "0";      // добавляем незначащий ноль к секундам
  Time += (String)ss; // добавляем секунды
  return Time;
} // функция возвращает строку

String CurrentTime()
{
  String str = ntp.timeToString();
  str.remove(5); // s = "01:02:00"
  return str;
}

/*
void fakeData() // генерирует случайные показания
{
  if (debug >= 2){
SteamTempN = random(80,90);
PipeTempN  = random(80,90);
TankTempN  = random(80,90);
  }
}
*/

//*******************************************************// debug

void debugTimePrint()
{
  /*
  #define DEBSTART if (debug==1) {debug_time_start = micros();}
  #define DEBSTOP if (debug==1) {debug_time_stop = micros() - debug_time_start; debugTimePrint(); }
  DEBSTART
  DEBSTOP
  timeloop_start = micros();
  timeloop_stop = micros() - timeloop_start;

 telnet.print("Причина перезагрузки");
telnetln.print(ESP.getResetReason());

  */

  if (debug >= 1)
  {

    double debug_time_stop_f = debug_time_stop;
    double timeloop_stop_f = timeloop_stop;
    telnet.print("Time: ");
    telnet.print(millis2time());
    telnet.print("    loop, ms: ");
    telnet.print(timeloop_stop_f / 1000);
    telnet.print("    debug, ms ");
    telnet.print(debug_time_stop_f / 1000);
    telnet.println();
  }
}

void beep()
{
  telnet.print(millis2time());
  telnet.print("   ");
  telnet.println(auto_status); // выводим сообщение на UART
  tone(buzzer_pin, 400, 200);
}

bool readValve()
{
  bool valve_status;
  valve_status = digitalRead(valve_pin);

  /*


  if (valve_invert == true)
  {
    valve_status = !digitalRead(valve_pin);
  }
  else
  {
    valve_status = digitalRead(valve_pin);
  }
  */
  return valve_status;
}

float corrTemp(float temp) // корректировка по давлению http://alcodistillers.ru/forum/viewtopic.php?pid=10973#p10973
{                          // функция принимает текущую температуру //1-2mc
  float temp0;
  if (BMP280_used == true && temp > 75 && press_correction == 1)
  {
    temp += (760 - atm_pressure) * 0.04; // приведение температуры к 760ммрт, при падении давления 1 мм относительно 760 температура падает на  0.04С
  }
  temp0 = round(temp * 20) / 20; // округление до 0.05
  return temp0;
}

float concFluid(float t) // Определение содержания спирта в кипящей жидкости,%об методом аппроксимации взята с форума http://labspirt.com/forum/index.php/topic,2403.15.html //70-90mc
{
  float Ti, f;
  Ti = (t - 89) / 6.49;
  f = (17.26 - Ti * (18.32 - Ti * (7.81 - Ti * (1.77 - Ti * (4.81 - Ti * (2.95 + Ti * (1.43 - Ti * (0.8 + 0.05 * Ti)))))))); // Содержание спирта в кубе,%об
  if (f <= 0 || f >= 100 || t >= 100 || t < 78)
  {
    f = 0;
  };
  return f;
}

float concSteam(float t) // Определение содержания спирта в парах,%об методом аппроксимации. Взято с онлайн-калькулятора https://planetcalc.ru/5992/  //20-60mc // 0,1mc
{
  float s;
  s = (-0.015146 * t * t * t + 3.875947 * t * t - 332.596610 * t + 9645.394183); // Содержание спирта в парах %об
  if (s <= 0 || s >= 100 || t >= 100 || t < 78)
  {
    s = 0;
  };
  return s;
}

void EEPROM_Reset()
{
  for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  beep();
  delay(500);
}

static uint64_t addr_64 = 00000000000000000000;
static String addr_str = "0x0000000000000000";

void findDS()
{
  GyverDS18Single dsSingle(ds_pin);
  dsSingle.reset();

  addr_64 = dsSingle.readAddress();

  if (addr_64)
  {
    telnet.print("  address find: ");
    addr_str = gds::addressToString(addr_64);
    telnet.println(addr_str);
    beep();
  }
  else
  {
    telnet.println("address no_find");
  }
}

void saveDS()
{
  ds_address[ds_index] = addr_64;

  switch (ds_index)
  {
  case 0:
    // addr = steam_addr;
    EEPROM.put(steam_addr, ds_address[0]);
  case 1:
    // addr = pipe_addr;
    EEPROM.put(pipe_addr, ds_address[1]);
  case 2:
    // addr = tank_addr;
    EEPROM.put(tank_addr, ds_address[2]);
  case 3:
    // addr = water_addr;
    EEPROM.put(water_addr, ds_address[3]);
  }
  EEPROM.commit();

  beep();
  // uint64_t read = 64;
  //  EEPROM.get(addr, read);
  telnet.print(" addr64 ");
  telnet.print(addr_64);
  telnet.print(" index ");
  telnet.print(ds_index);
  telnet.println();
}

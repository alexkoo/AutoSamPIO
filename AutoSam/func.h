#ifndef func_h
#define func_h
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
  String str = ntp.timeString();
  str.remove(5); // s = "01:02:00"
  return str;
}

/*
void fake_data() // генерирует случайные показания
{
  if (debug >= 2){
SteamTempN = random(80,90);
PipeTempN  = random(80,90);
TankTempN  = random(80,90);
  }
}
*/

void debugTimePrint()
{
  /*
  #define DEBSTART if (debug==1) {debug_time_start = micros();}
  #define DEBSTOP if (debug==1) {debug_time_stop = micros() - debug_time_start; debugTimePrint(); }
  DEBSTART
  DEBSTOP
  timeloop_start = micros();
  timeloop_stop = micros() - timeloop_start;
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
  tone(buz, 400, 200);
}

bool readValve()
{
  bool valve_status;
  valve_status = digitalRead(valve);

  /*


  if (valve_invert == true)
  {
    valve_status = !digitalRead(valve);
  }
  else
  {
    valve_status = digitalRead(valve);
  }
  */
  return valve_status;
}

/*



void findDS()
{
  byte i;
  // byte j;
  byte addr[8];
  byte numDS = sensors.getDeviceCount();
  telnet.println("Поиск датчиков ");
  telnet.print("Найдено датчиков ");
  telnet.println(numDS); // вывод на экран общего количества найденых датчиков
  if (numDS == 0)
  {
    telnet.print("Error, no sensors found, please check");
    return;
  }
  for (i = 0; i < numDS; i++)
  {
    if (!oneWire.search(addr))
    {
      telnet.println(" No more addresses.");
      telnet.println();
      oneWire.reset_search();
      delay(250);
      return;
    }
    telnet.print(" ROM =");
    for (i = 0; i < 8; i++)
    {
      telnet.write(' ');
      telnet.print(addr[i], HEX);
    }
  }
}

void printAddress(DeviceAddress deviceAddress) // функция печати адреса DS18B20
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16)
      Serial.print("0"); // вставляем незначащие нули
    Serial.print(deviceAddress[i], HEX);
  }
}
*/

float corrTemp(float temp) // корректировка по давлению http://alcodistillers.ru/forum/viewtopic.php?pid=10973#p10973
{                          // функция принимает текущую температуру //1-2mc
  if (BMP280_used == true && temp > 75 && press_correction == 1)
  {
    temp += (760 - atm_pressure) * 0.04; // приведение температуры к 760ммрт, при падении давления 1 мм относительно 760 температура падает на  0.04С
  }
  return temp;
}

float concFluid(float t) // Определение содержания спирта в кипящей жидкости,%об методом аппроксимации взята с форума http://labspirt.com/forum/index.php/topic,2403.15.html //70-90mc
{
  float Ti, f;
  Ti = (t - 89) / 6.49;
  f = (17.26 - Ti * (18.32 - Ti * (7.81 - Ti * (1.77 - Ti * (4.81 - Ti * (2.95 + Ti * (1.43 - Ti * (0.8 + 0.05 * Ti)))))))); //Содержание спирта в кубе,%об
  if (f <= 0 || f >= 100 || t >= 100 || t < 78)
  {
    f = 0;
  };
  return f;
}

float concSteam(float t) //Определение содержания спирта в парах,%об методом аппроксимации. Взято с онлайн-калькулятора https://planetcalc.ru/5992/  //20-60mc // 0,1mc
{
  float s;
  s = (-0.015146 * t * t * t + 3.875947 * t * t - 332.596610 * t + 9645.394183); //Содержание спирта в парах %об
  if (s <= 0 || s >= 100 || t >= 100 || t < 78)
  {
    s = 0;
  };
  return s;
}

float EEPROM_Read(int addr) // чтение данных из EEPROM (адрес)
{
  byte raw[4];
  for (byte i = 0; i < 4; i++)
    raw[i] = EEPROM.read(addr + i);
  float &num = (float &)raw;
  return num;
}

void EEPROM_Write(int addr, float num) // Запись данных в EEPROM (адрес, значение)
{
  if (EEPROM_Read(addr) != num)
  { //если сохраняемое отличается
    byte raw[4];
    (float &)raw = num;
    for (byte i = 0; i < 4; i++)
      EEPROM.write(addr + i, raw[i]);
  }
  EEPROM.commit();
}

#endif

#include "header.h"

void findds()
{
  byte i;
  byte j;
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
  for (j = 0; i < numDS; i++)
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

// корректировка по давлению http://alcodistillers.ru/forum/viewtopic.php?pid=10973#p10973
float corrTemp(float temp)
{ // функция принимает текущую температуру
  if (BMP280 == true && temp > 75 && press_corr == 1)
  {
    temp += (760 - atm_pressure) * 0.04; // приведение температуры к 760ммрт, при падении давления 1 мм относительно 760 температура падает на  0.04С
  }
  return temp;
}

float ConcFluid(float t)
{ // Определение содержания спирта в кипящей жидкости,%об методом аппроксимации взята с форума http://labspirt.com/forum/index.php/topic,2403.15.html
  float Ti, f;
  Ti = (t - 89) / 6.49;
  f = (17.26 - Ti * (18.32 - Ti * (7.81 - Ti * (1.77 - Ti * (4.81 - Ti * (2.95 + Ti * (1.43 - Ti * (0.8 + 0.05 * Ti)))))))); //Содержание спирта в кубе,%об
  if (f <= 0 || f >= 100 || t >= 100 || t < 78)
  {
    f = 0;
  };
  return f;
}

float ConcSteam(float t)
{ //Определение содержания спирта в парах,%об методом аппроксимации. Взято с онлайн-калькулятора https://planetcalc.ru/5992/
  float s;
  s = (-0.015146 * t * t * t + 3.875947 * t * t - 332.596610 * t + 9645.394183); //Содержание спирта в парах %об
  if (s <= 0 || s >= 100 || t >= 100 || t < 78)
  {
    s = 0;
  };
  return s;
}

// чтение EEPROM

float EEPROM_read(int addr, byte bytn) // чтение данных из EEPROM (адрес, количесство байтов)
{
  if (bytn == 2)
  {
    byte raw[2];
    for (byte i = 0; i < 2; i++)
      raw[i] = EEPROM.read(addr + i);
    int &num = (int &)raw;
    return num;
  }

  if (bytn == 4)
  {
    byte raw[4];
    for (byte i = 0; i < 4; i++)
      raw[i] = EEPROM.read(addr + i);
    float &num = (float &)raw;
    return num;
  }
  else
    return 0;
}
/*
float EEPROM_int_read(int addr)
{
  byte raw[2];
  for (byte i = 0; i < 2; i++)
    raw[i] = EEPROM.read(addr + i);
  int &num = (int &)raw;
  return num;
}

float EEPROM_float_read(int addr)
{
  byte raw[4];
  for (byte i = 0; i < 4; i++)
    raw[i] = EEPROM.read(addr + i);
  float &num = (float &)raw;
  return num;
}

  void EEPROM_addr_read(int ad ) // на вход адрес eeprom и индекс датчика
  {
  for (byte f = 0; f < 8; f++) {
    sens0[f] = EEPROM.read(ad + f);
   // addr[i][f] = EEPROM.read(ad + f);
  }
  }


*/

// запись EEPROM

void EEPROM_write(int addr, float num, byte bytn) // Запись данных в EEPROM (адрес, число, количесство байтов(int=2, float=4))
{                                                
  if (bytn = 2) // запись int
  { int num = num;
    if (EEPROM_read(addr, 2) != num)
    { //если сохраняемое отличается
      byte raw[2];
      (int &)raw = num;
      for (byte i = 0; i < 2; i++)
        EEPROM.write(addr + i, raw[i]);
    }
    EEPROM.commit();
  }
  if (bytn = 4) // запись float
  {
    if (EEPROM_read(addr, 4) != num)
    { //если сохраняемое отличается
      byte raw[4];
      (float &)raw = num;
      for (byte i = 0; i < 4; i++)
        EEPROM.write(addr + i, raw[i]);
    }
    EEPROM.commit();
  }
}
/*
void EEPROM_int_write(int addr, int num) // Запись данных в EEPROM (адрес, число, количесство байтов)
{                                        // запись int
}

void EEPROM_float_write(int addr, float num)
{ // запись float
  if (EEPROM_read(addr, 4) != num)
  { //если сохраняемое отличается
    byte raw[4];
    (float &)raw = num;
    for (byte i = 0; i < 4; i++)
      EEPROM.write(addr + i, raw[i]);
  }
  EEPROM.commit();
}

*/
void EEPROM_addr_write(int addr, byte *data) // на вход адрес eeprom и адрес датчика
{
  for (int i = 0; i < 8; i++)
    EEPROM.write(addr++, *data++);
}

/*
   Готовая функция для вычисления среднего арифметического
   Принимает новые значения, суммирует их в своём массиве

  // функция вычисления среднего
  #define NUM_AVER 10       // выборка (из скольки усредняем)
  long average;             // перем. среднего
  int valArray[NUM_AVER];   // массив
  byte idx = 0;             // индекс


  int middleArifm(int newVal) {       // принимает новое значение
  valArray[idx] = newVal;           // пишем каждый раз в новую ячейку
  if (++idx >= NUM_AVER) idx = 0;   // перезаписывая самое старое значение
  average = 0;                      // обнуляем среднее
  for (int i = 0; i < NUM_AVER; i++) {
    average += valArray[i];         // суммируем
  }
  average /= NUM_AVER;              // делим
  return average;                   // возвращаем
  }
*/

/*

  Функция ESP.restart() перезапускает ESP8266.
  Функция ESP.getResetReason() возвращает строку, описывающую причину последнего сброса (в удобочитаемом формате).
  Функция ESP.getFreeHeap() возвращает размер свободной памяти.
  Функция ESP.getChipId() возвращает ID чипа ESP8266 в виде целого 32-битного значения.
  Функция ESP.getCoreVersion() возвращает строку с версией аддона.
  Функция ESP.SdkVersion() возвращает версию SDK в виде данных типа char.
  Функция ESP.CpuFreqMHz() возвращает частоту процессора в виде 8-битного беззнакового целочисленного значения (в МГц).
  Функция ESP.getSketchSize() возвращает размер скетча, в данный момент загруженного на ESP8266, в виде 32-битного целочисленного значения.
  Функция ESP.getFreeSketchSpace() возвращает свободное место для загрузки скетча в виде 32-битного целочисленного значения.
  Функция ESP.getSketchMD5() возвращает строку (с символами в нижнем регистре), содержащую MD5-хэш скетча, в данный момент загруженного на ESP8266.
  Функция ESP.getFlashChipId() возвращает ID флэш-памяти чипа в виде 32-битного целочисленного значения.
  Функция ESP.getFlashChipSize() возвращает размер flash-памяти чипа (в байтах), каким его видит SDK (может быть меньше настоящего размера).
  Функция ESP.getFlashChipRealSize() возвращает настоящий размер flash-памяти (в байтах), основанный на ID flash-памяти чипа.
  Функция ESP.getFlashChipSpeed(void) возвращает частоту flash-памяти чипа (в Гц).
  Функция ESP.getCycleCount() возвращает количество командных циклов процессора в момента запуска чипа. Возвращаемое значение имеет вид 32-битного беззнакового значения. Эта функция полезна для точной синхронизации очень коротких действий (вроде тех, что используются при использовании техники «bit-banging»).
  Функцию ESP.getVcc() можно использовать для измерения питающего напряжения. Для использования этой функции необходимо, чтобы ESP при запуске нужным образом перенастроил АЦП. Для этого добавьте в верхнюю часть скетча строчку...
  ADC_MODE(ADC_VCC);
*/

/*
  void SensSearh(void) {
  byte i;
  byte addr[8];

  if (!ds.search(addr)) {
    telnet.println(" No more addresses.");
    telnet.println();
    ds.reset_search();
    delay(250);
    return;
  }
  Serial.print(" ROM =");
  for (i = 0; i < 8; i++) {
    telnet.write(' ');
    telnet.print(addr[i], HEX);
  }
  }

  void readSens() {
  telnet.println("Поиск датчиков ");
  sensors.begin();                                   // Инициализация датчиков DS18B20
  byte  numberOfDevices = sensors.getDeviceCount();
  telnet.print("Найдено датчиков ");
  telnet.println(numberOfDevices);                  // вывод на экран общего количества найденых датчиков

  sensors.getAddress(sens0, 0);
  sensors.getAddress(sens1, 1);
  sensors.getAddress(sens2, 2);
  sensors.getAddress(sens3, 3);

  EEPROM_addr_write(18, sens0);
  EEPROM_addr_write(26, sens1);
  EEPROM_addr_write(34, sens2);
  EEPROM_addr_write(42, sens3);


  sensors.setResolution(SteamSensor, 12);                      // устанавливаем разрешение для датчика 0
  sensors.setResolution(PipeSensor, 12);                       // устанавливаем разрешение для датчика 1
  sensors.setResolution(WaterSensor, 12);                      // устанавливаем разрешение для датчика 2
  sensors.setResolution(TankSensor, 12);                       // устанавливаем разрешение для датчика 3

  /*
  telnet.println ("Sensors Address: ");                      // пишем адрес и разрешение датчика 0
  telnet.println (printAddress(sens0));
  telnet.println (printAddress(sens1));
  telnet.println (printAddress(sens2));
  telnet.println (printAddress(sens3));

  }
*/

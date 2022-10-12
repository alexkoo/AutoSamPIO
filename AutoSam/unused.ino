

  //********************************************
  // Обрабатываем нажатия кнопки переключения экранов дисплея

/*

float EEPROM_read(int addr) // чтение данных из EEPROM (адрес, количесство байтов)
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

*/

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


void EEPROM_write(int addr, float num) // Запись данных в EEPROM (адрес, число, количесство байтов(int=2, float=4))
{
  if (EEPROM_read(addr) != num)
  { //если сохраняемое отличается
    byte raw[4];
    (float &)raw = num;
    for (byte i = 0; i < 4; i++)
      EEPROM.write(addr + i, raw[i]);
  }
  EEPROM.commit();
}

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


  telnet.println ("Sensors Address: ");                      // пишем адрес и разрешение датчика 0
  telnet.println (printAddress(sens0));
  telnet.println (printAddress(sens1));
  telnet.println (printAddress(sens2));
  telnet.println (printAddress(sens3));

  }






  if (digitalRead(button) == LOW && pressed == 0) //если кнопка нажата и переменная pressed равна 0 , то ...
  {
    pressed = 1;   //это нужно для того, чтобы с каждым нажатием кнопки происходило только одно действие плюс защита от "дребезга"
    lcd_num++;     // увеличиваем номер экрана на 1
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
*/


/*

 //Convert device id to String
String GetAddressToString(DeviceAddress deviceAddress) {
  String str = "";
  for (uint8_t i = 0; i < 8; i++) {
    if ( deviceAddress[i] < 16 ) str += String(0, HEX);
    str += String(deviceAddress[i], HEX);
  }
  return str;
}



  float getParVolPercent(float temp) {
  if (hasBmp180 & bmp180Pressure > 0) {
    temp += (760 - bmp180Pressure) * 0.04; // приведение температуры к 760ммрт, при падении давления 1 мм относительно 760 температура падает на  0.04С
  }
  if (temp >= 100 || temp <= 88) return 0;
  uint8_t idx = temp - 88; //  разница Т от первого значения таблицы
  float ret = 0.1 * par[idx] - 0.1 * (par[idx] - par[idx + 1]) * (temp - 88 - idx); // хз как работает
  return ret;
  }

  float getBakVolPercent(float temp) {
  if (hasBmp180 & bmp180Pressure > 0) {
    temp += (760 - bmp180Pressure) * 0.04;
  }
  if (temp >= 100 || temp < 88) return 0;
  uint8_t idx = temp - 88;
  float ret = 0.1 * bak[idx] - 0.1 * (bak[idx] - bak[idx + 1]) * (temp - 88 - idx);
  return ret;
  }



if (debug == 1)
  {
    if (millis() - debug_time >= 1000)
    {                        // выполняется раз в 1000 мс
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



 
void EEPROM_float_write(int addr, float val_){     //Запись в ЕЕПРОМ
скрин с проектом прилагаю ,



float EEPROM_float_read(int addr){               //Чтение из ЕЕПРОМ
  EEPROM.begin(512);              //Начало работы с EEPROM модуля esp8266
  byte x[4];
  for(byte i = 0; i < 4; i++) x[i] = EEPROM.read(i+addr);
  float *y = (float *)&x;
    EEPROM.commit();                //Окончание работы после записи с EPROM модуля esp8266
  return y[0];
}

@Thorn зачем вам float? Умножьте температуру на 10 или на 100 и сохраняйте в int32 и даже int16 хватит


// время выполнения функции
unsigned long start_time = micros(); 
// 
//Ваш Код
// 
unsigned long end_time = micros();
unsigned long delta_time = end - start;
telnet.print("Time , ms: ");
telnet.println(delta_time);
// время выполнения функции


telnet 800-900
2 telnet 1000 1200



  // корректировка по давлению http://alcodistillers.ru/forum/viewtopic.php?pid=10973#p10973   alcodistillers.ru/forum/viewtopic.php?id=1135&p=2
  // массив, где хранится крепость*10 ( темп от 88)
  uint8_t bak[] = {219, 191, 165, 143, 122, 102, 85, 69, 53, 39, 25, 12, 0};
  uint16_t par[] = {689, 667, 641, 613, 579, 536, 490, 436, 368, 295, 207, 108, 0};
































  */
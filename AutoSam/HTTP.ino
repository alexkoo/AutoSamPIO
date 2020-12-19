
#include "header.h"

void HTTP_init(void)
{            // функция инициализации HTTP
  FS_init(); // Включаем работу с файловой системой

  //Выполнение команды из браузера
  HTTP.on("/Del1", handle_Del1);     // обрашение к уставке SteamTemp через web интерфейс
  HTTP.on("/Del2", handle_Del2);     // обрашение к уставке PipeTemp(2/3) через web интерфейс
  HTTP.on("/SetMD", handle_setMD);   // обрашение к настройкам через web интерфейс (режим)
  HTTP.on("/SetMB", handle_setMB);   // обрашение к настройкам через web интерфейс (Мин. температура бака)
  HTTP.on("/SetMT", handle_setMT);   // обрашение к настройкам через web интерфейс (Макс. температура бака)
  HTTP.on("/SetSM", handle_setSM);   // обрашение к настройкам через web интерфейс (Макс. температура пара)
  HTTP.on("/SetD0", handle_setD0);   // обрашение к настройкам через web интерфейс (Скорость изменения температуры))
//HTTP.on("/press", handle_press_corr);               // Коррекция давления
  HTTP.on("/reset", handle_reset);   // перезагрузка
  HTTP.on("/button", handle_button); // обрашение к кнопкам через web интерфейс
  HTTP.on("/data.json", handleData); // формирование json файла для передачи данных в web интерфейс
  HTTP.begin();                      // Запускаем HTTP сервер
}
//**************************************************************************************************
void handle_Del1()
{                                              // функция изменения уставок с web страницы //SteamTemp
  float delta1 = HTTP.arg("delta1").toFloat(); // получаем от клиента строку с дельтой
  float delay1 = HTTP.arg("delay1").toInt();   // получаем от клиента строку с задержкой
  if (delta1 == 0)
    SetTemp1 = 0; // устанавливаем уставку 0
  else
    SetTemp1 = SteamTemp + delta1; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта

  Serial.print("Delta1="); // выводим новое значение уставки на UART
  Serial.println(delta1);
  Serial.print("Delay1="); // выводим новое значение задержки на UART
  Serial.println(delay1);
  Serial.print("SetTemp1="); // выводим новое значение уставки на UART
  Serial.println(SetTemp1);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//**************************************************************************************************
void handle_Del2()
{                                              // функция изменения уставок с web страницы //PipeTemp(2/3)
  float delta2 = HTTP.arg("delta2").toFloat(); // получаем от клиента строку с дельтой
  float delay2 = HTTP.arg("delay2").toInt();   // получаем от клиента строку с задержкой
  if (delta2 == 0)
    SetTemp2 = 0; // устанавливаем уставку 0
  else
    SetTemp2 = PipeTemp + delta2; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта

  Serial.print("Delta2="); // выводим новое значение уставки на UART
  Serial.println(delta2);
  Serial.print("Delay2="); // выводим новое значение задержки на UART
  Serial.println(delay2);
  Serial.print("SetTemp2="); // выводим новое значение уставки на UART
  Serial.println(SetTemp2);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}
//***************************************************************************************************
void handle_button()
{                                        // функция управления клапаном с web страницы
  int state = HTTP.arg("state").toInt(); // получаем от клиента строку с номером нажатой кнопки

  if (state == 6)
  { // если передан номер кнопки 6 "Открыть"
    valveset = true;
  }
  if (state == 7)
  { // если передан номер кнопки 7 "Закрыть"
    valveset = false;
  }
  if (state == 10)
  { // если передан номер кнопки 10 "Поиск датчиков"
    findds();
  }

  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//***************************************************************************************************

void handle_setMD()
{                                        // функция изменения настроек с web страницы (режим)
  autosam_mode = HTTP.arg("modesam").toInt(); // получаем от клиента строку с режимом
  HTTP.send(200, "text/plain", "OK");    // передаём ответ
  EEPROM.write(autosam_mode_addr, autosam_mode);
  EEPROM.commit();
}
void handle_setMB()
{                                          // функция изменения настроек с web страницы (Мин. температура)
  min_hot_temp = HTTP.arg("minboil").toFloat(); // получаем от клиента строку с температурой
  HTTP.send(200, "text/plain", "OK");      // передаём ответ
  EEPROM_float_write(min_hot_temp_addr, min_hot_temp);
}

void handle_setMT()
{                                          // функция изменения настроек с web страницы (Макс. температура бака)
  max_tank_temp = HTTP.arg("maxtank").toFloat(); // получаем от клиента строку с максимальной температурой бака
  HTTP.send(200, "text/plain", "OK");      // передаём ответ
  EEPROM_float_write(max_tank_temp_addr, max_tank_temp);
}

void handle_setSM()
{                                         // функция изменения настроек с web страницы
  max_steam_temp = HTTP.arg("stmax").toFloat(); // получаем от клиента строку с максимальной температурой Пара
  HTTP.send(200, "text/plain", "OK");     // передаём ответ
  EEPROM_float_write(max_steam_temp_addr, max_steam_temp);
}

void handle_setD0()
{                                         // функция изменения настроек с web страницы (скорость изменения температуры)
  heating_rate = HTTP.arg("delta0").toFloat(); // получаем от клиента строку с дельтой (скорость изменения температуры)
  HTTP.send(200, "text/plain", "OK");     // передаём ответ
  EEPROM_float_write(heating_rate_addr, heating_rate);
}

void handle_press_corr() {                             // функция изменения настроек с web страницы (Коррекция давления)
  press_corr = HTTP.arg("press_corr").toInt();          // получаем от клиента строку с режимом
  HTTP.send(200, "text/plain", "OK");            // передаём ответ
  EEPROM.write(press_corr_addr, press_corr);
  EEPROM.commit();
}

void handle_reset()
{ // функция перезагрузки с web страницы
  ESP.restart();
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//*****************************************************************************************************
void handleData() // функция передачи файла data.json клиенту
{
  String json = "{";                              // начинаем с открывающейся фигурной скобки
  json += "\"CT\":\"" + String(CurrentTime());    // кавычки экранируются!
  json += "\",\"RT\":\"" + String(millis2time()); // только 2 символа
  json += "\",\"ST\":\"" + String(SteamTemp);
  json += "\",\"SF\":\"" + String(SteamTempF);
  json += "\",\"SS\":\"" + String(SteamTempS);
  json += "\",\"PT\":\"" + String(PipeTemp);
  json += "\",\"PF\":\"" + String(PipeTempF);
  json += "\",\"PS\":\"" + String(PipeTempS);
  json += "\",\"WT\":\"" + String(WaterTemp);
  json += "\",\"TT\":\"" + String(TankTemp);
  json += "\",\"TF\":\"" + String(TankTempF);
  json += "\",\"TS\":\"" + String(TankTempS);
  json += "\",\"AP\":\"" + String(Apressure);
  json += "\",\"AT\":\"" + String(AirTemp);
  json += "\",\"FM\":\"" + String(freeMem);
  json += "\",\"WA\":\"" + String(warning);
  json += "\",\"DS\":\"" + String(deltaTs);
  json += "\",\"DP\":\"" + String(deltaTp);
  json += "\",\"DT\":\"" + String(deltaTt);
  json += "\",\"VR\":\"" + String(ver);
  json += "\",\"MD\":\"" + String(autosam_mode);
  json += "\",\"MB\":\"" + String(min_hot_temp);
  json += "\",\"MT\":\"" + String(max_tank_temp);
  json += "\",\"D0\":\"" + String(heating_rate);
  json += "\",\"SM\":\"" + String(max_steam_temp);
  json += "\",\"S1\":\"" + String(SetTemp1);
  json += "\",\"S2\":\"" + String(SetTemp2);
  json += "\",\"AS\":\"" + String(AutoStatus);
  json += "\",\"VS\":\"" + String();
  if (digitalRead(valve))
    json += " ЗАКРЫТ ";
  else
    json += " ОТКРЫТ ";
  json += "\"}";                     // не забудем закрыть фигурную скобку!
  HTTP.send(200, "text/json", json); // передаём json
}

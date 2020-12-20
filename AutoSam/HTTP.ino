
#include "header.h"

void HTTP_init(void)
{            // функция инициализации HTTP
  FS_init(); // Включаем работу с файловой системой

  //Выполнение команды из браузера
  HTTP.on("/button", handle_Button);         // обрашение к кнопкам через web интерфейс
  HTTP.on("/DelS", handle_DeltaSteam);       // обрашение к уставке SteamTemp через web интерфейс
  HTTP.on("/DelP", handle_DeltaPipe);        // обрашение к уставке PipeTemp(2/3) через web интерфейс

  HTTP.on("/SetMD", handle_setMode);         // обрашение к настройкам через web интерфейс (режим)
  HTTP.on("/SetMB", handle_setMinTemp);      // обрашение к настройкам через web интерфейс (Мин. температура бака)
  HTTP.on("/SetMT", handle_setMaxTankTemp);  // обрашение к настройкам через web интерфейс (Макс. температура бака)
  HTTP.on("/SetSM", handle_setMaxSteamTemp); // обрашение к настройкам через web интерфейс (Макс. температура пара)
  HTTP.on("/SetD0", handle_setHeatingRate);  // обрашение к настройкам через web интерфейс (Скорость изменения температуры))
  HTTP.on("/press", handle_press_corr);      // Коррекция давления
  
  HTTP.on("/data.json", handleData); // формирование json файла для передачи данных в web интерфейс
  HTTP.begin();                      // Запускаем HTTP сервер
}

//***************************************************************************************************
void handle_Button()
{                                               // функция управления клапаном с web страницы
  int button_state = HTTP.arg("state").toInt(); // получаем от клиента строку с номером нажатой кнопки

  if (button_state == 6)
  { // если передан номер кнопки 6 "Открыть"
    valve_manual = true;
  }
  if (button_state == 7)
  { // если передан номер кнопки 7 "Закрыть"
    valve_manual = false;
  }
  if (button_state == 41)
  { // если передан номер кнопки 41 "Поиск датчиков"
    findDS();
  }
  if (button_state == 42)
  { // если передан номер кнопки 42 "reset"
    ESP.restart();
  }

  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//**************************************************************************************************
void handle_DeltaSteam()
{                                                   // функция изменения уставок с web страницы //SteamTemp
  float delta_steam = HTTP.arg("delta_s").toFloat(); // получаем от клиента строку с дельтой
  delay_steam = HTTP.arg("delay_s").toInt();         // получаем от клиента строку с задержкой
  if (delta_steam == 0)
    set_temp_steam = 0; // устанавливаем уставку 0
  else
    set_temp_steam = SteamTemp + delta_steam; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта

  Serial.print("Delta1="); // выводим новое значение уставки на UART
  Serial.println(delta_steam);
  Serial.print("delay_steam="); // выводим новое значение задержки на UART
  Serial.println(delay_steam);
  Serial.print("set_temp_steam="); // выводим новое значение уставки на UART
  Serial.println(set_temp_steam);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//**************************************************************************************************
void handle_DeltaPipe()
{                                                  // функция изменения уставок с web страницы //PipeTemp(2/3)
  float delta_pipe = HTTP.arg("delta_p").toFloat(); // получаем от клиента строку с дельтой
  delay_pipe = HTTP.arg("delay_p").toInt();         // получаем от клиента строку с задержкой
  if (delta_pipe == 0)
    set_temp_pipe = 0; // устанавливаем уставку 0
  else
    set_temp_pipe = PipeTemp + delta_pipe; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта

  Serial.print("Delta pipe="); // выводим новое значение уставки на UART
  Serial.println(delta_pipe);
  Serial.print("delay_pipe="); // выводим новое значение задержки на UART
  Serial.println(delay_pipe);
  Serial.print("set_temp_pipe="); // выводим новое значение уставки на UART
  Serial.println(set_temp_pipe);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}
//***************************************************************************************************

void handle_setMode()
{                                             // функция изменения настроек с web страницы (режим)
  autosam_mode = HTTP.arg("modesam").toInt(); // получаем от клиента строку с режимом
  HTTP.send(200, "text/plain", "OK");         // передаём ответ
  EEPROM.write(autosam_mode_addr, autosam_mode);
  EEPROM.commit();
}
//***************************************************************************************************

void handle_setMinTemp()
{                                               // функция изменения настроек с web страницы (Мин. температура)
  min_hot_temp = HTTP.arg("minboil").toFloat(); // получаем от клиента строку с температурой
  HTTP.send(200, "text/plain", "OK");           // передаём ответ
  EEPROM_write(min_hot_temp_addr, min_hot_temp, 4);
}

void handle_setMaxTankTemp()
{                                                // функция изменения настроек с web страницы (Макс. температура бака)
  max_tank_temp = HTTP.arg("maxtank").toFloat(); // получаем от клиента строку с максимальной температурой бака
  HTTP.send(200, "text/plain", "OK");            // передаём ответ
  EEPROM_write(max_tank_temp_addr, max_tank_temp, 4);
}

void handle_setMaxSteamTemp()
{                                               // функция изменения настроек с web страницы
  max_steam_temp = HTTP.arg("stmax").toFloat(); // получаем от клиента строку с максимальной температурой Пара
  HTTP.send(200, "text/plain", "OK");           // передаём ответ
  EEPROM_write(max_steam_temp_addr, max_steam_temp, 4);
}

void handle_setHeatingRate()
{                                              // функция изменения настроек с web страницы (скорость изменения температуры)
  heating_rate = HTTP.arg("delta0").toFloat(); // получаем от клиента строку с дельтой (скорость изменения температуры)
  HTTP.send(200, "text/plain", "OK");          // передаём ответ
  EEPROM_write(heating_rate_addr, heating_rate, 4);
}

void handle_press_corr()
{                                              // функция изменения настроек с web страницы (Коррекция давления)
  press_corr = HTTP.arg("press_corr").toInt(); // получаем от клиента строку с режимом
  HTTP.send(200, "text/plain", "OK");          // передаём ответ
  EEPROM.write(press_corr_addr, press_corr);
  EEPROM.commit();
}

//*****************************************************************************************************
void handleData() // функция передачи файла data.json клиенту
{
  String json = "{";                              // начинаем с открывающейся фигурной скобки
  json += "\"CT\":\"" + String(CurrentTime());    // кавычки экранируются!
  json += "\",\"RT\":\"" + String(millis2time()); // только 2 символа
  json += "\",\"VR\":\"" + String(VER);
  json += "\",\"MD\":\"" + String(autosam_mode);
  //head
  json += "\",\"ST\":\"" + String(SteamTemp);
  json += "\",\"SF\":\"" + String(SteamTempVolF);
  json += "\",\"SS\":\"" + String(SteamTempVolS);
  json += "\",\"PT\":\"" + String(PipeTemp);
  json += "\",\"PF\":\"" + String(PipeTempVolF);
  json += "\",\"PS\":\"" + String(PipeTempVolS);
  json += "\",\"WT\":\"" + String(WaterTemp);
  json += "\",\"TT\":\"" + String(TankTemp);
  json += "\",\"TF\":\"" + String(TankTempVolF);
  json += "\",\"TS\":\"" + String(TankTempVolS);
  json += "\",\"HS\":\"" + String(heating_rate_steam);
  json += "\",\"HP\":\"" + String(heating_rate_pipe);
  json += "\",\"HT\":\"" + String(heating_rate_tank);
  json += "\",\"IS\":\"" + String(set_temp_steam);
  json += "\",\"IP\":\"" + String(set_temp_pipe);
  json += "\",\"AP\":\"" + String(atm_pressure);
  json += "\",\"AT\":\"" + String(air_temp);
  json += "\",\"AS\":\"" + String(auto_status);
  json += "\",\"WA\":\"" + String(warning);
  json += "\",\"FM\":\"" + String(free_mem);

  json += "\",\"MB\":\"" + String(min_hot_temp);
  json += "\",\"MT\":\"" + String(max_tank_temp);
  json += "\",\"D0\":\"" + String(heating_rate);
  json += "\",\"SM\":\"" + String(max_steam_temp);


  json += "\",\"VS\":\"" + String();
  if (digitalRead(valve))
    json += " ЗАКРЫТ ";
  else
    json += " ОТКРЫТ ";
  json += "\"}";                     // не забудем закрыть фигурную скобку!
  HTTP.send(200, "text/json", json); // передаём json
}

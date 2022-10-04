#include "header.h"

void HTTP_init(void) // функция инициализации HTTP
{
  FS_init(); // Включаем работу с файловой системой

  //Выполнение команды из браузера
  HTTP.on("/button", handle_Button); // обрашение к кнопкам через web интерфейс
  HTTP.on("/SetForm", handle_SetForm);
  HTTP.on("/DelS", handle_DeltaSteam); // обрашение к уставке SteamTemp через web интерфейс
  HTTP.on("/DelP", handle_DeltaPipe);  // обрашение к уставке PipeTemp(2/3) через web интерфейс
  HTTP.on("/data.json", handleData);   // формирование json файла для передачи данных в web интерфейс
  HTTP.begin();                        // Запускаем HTTP сервер
}

void handle_SetForm()
{ // функция изменения настроек с web страницы

  autosam_mode = HTTP.arg("autosam_mode_h").toInt(); // получаем от клиента строку с режимом
  EEPROM.write(autosam_mode_addr, autosam_mode);

  min_hot_temp = HTTP.arg("min_hot_temp_h").toFloat();
  EEPROM_write(min_hot_temp_addr, min_hot_temp, 4);

  max_tank_temp = HTTP.arg("max_tank_temp_h").toFloat();
  EEPROM_write(max_tank_temp_addr, max_tank_temp, 4);

  max_steam_temp = HTTP.arg("max_steam_temp_h").toFloat();
  EEPROM_write(max_steam_temp_addr, max_steam_temp, 4);

  heating_rate = HTTP.arg("heating_rate_h").toFloat();
  EEPROM_write(heating_rate_addr, heating_rate, 4);

  // press_corr = HTTP.arg("press_corr").toInt();
  // EEPROM.write(press_corr_addr, press_corr);

  HTTP.send(200, "text/plain", "OK"); // передаём ответ
  EEPROM.commit();
}

//***************************************************************************************************
void handle_Button()
{                                               // функция управления клапаном с web страницы
  int button_state = HTTP.arg("state").toInt(); // получаем от клиента строку с номером нажатой кнопки

  if (button_state == 6) // если передан номер кнопки 6 "Открыть"
  {
    valve_auto_mode = false;
    digitalWrite(valve, ON);
    auto_status = "Opened, Man";
    beep();
  }
  if (button_state == 7) // если передан номер кнопки 7 "Закрыть"
  {
    valve_auto_mode = false;
    digitalWrite(valve, OFF); // выключаем клапан
    auto_status = "Closed, Man";
    beep();
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
{                                                    // функция изменения уставок с web страницы //SteamTemp
  float delta_steam = HTTP.arg("delta_s").toFloat(); // получаем от клиента строку с дельтой
  delay_steam = HTTP.arg("delay_s").toInt();         // получаем от клиента строку с задержкой
  if (delta_steam == 0)
  {
    set_temp_steam = 0;                           // устанавливаем уставку 0
     if (set_temp_pipe == 0) // если  обе уставки равны 0 (ручное управление)
    {
      valve_auto_mode = false;
    }
  }
  else
    set_temp_steam = SteamTemp + delta_steam; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта
  valve_auto_mode = true;
  telnet.print("DeltaSteam="); // выводим новое значение уставки на UART
  telnet.println(delta_steam);
  telnet.print("set_temp_steam="); // выводим новое значение уставки на UART
  telnet.println(set_temp_steam);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//**************************************************************************************************
void handle_DeltaPipe()
{                                                   // функция изменения уставок с web страницы //PipeTemp(2/3)
  float delta_pipe = HTTP.arg("delta_p").toFloat(); // получаем от клиента строку с дельтой
  delay_pipe = HTTP.arg("delay_p").toInt();         // получаем от клиента строку с задержкой
  if (delta_pipe == 0)
  {
    set_temp_pipe = 0;       // устанавливаем уставку 0
    if (set_temp_steam == 0) // если  обе уставки равны 0 (ручное управление)
    {
      valve_auto_mode = false;
    }
  }

  else
    set_temp_pipe = PipeTemp + delta_pipe; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта
  valve_auto_mode = true;
  telnet.print("Delta pipe="); // выводим новое значение уставки на UART
  telnet.println(delta_pipe);
  telnet.print("delay_pipe="); // выводим новое значение задержки на UART
  telnet.println(delay_pipe);
  telnet.print("set_temp_pipe="); // выводим новое значение уставки на UART
  telnet.println(set_temp_pipe);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}
//***************************************************************************************************

void handleData() // функция передачи файла data.json клиенту
{
  String json = "{";                                // начинаем с открывающейся фигурной скобки
  json += "\"TIME\":\"" + String(CurrentTime());    // кавычки экранируются!
  json += "\",\"RTIM\":\"" + String(millis2time()); // только 2 символа
  json += "\",\"VER\":\"" + String(VER);
  json += "\",\"MOD\":\"" + String(autosam_mode);
  //head
  json += "\",\"ST\":\"" + String(SteamTemp);
  json += "\",\"SF\":\"" + String(SteamTempVolF);
  json += "\",\"SS\":\"" + String(SteamTempVolS);
  json += "\",\"PT\":\"" + String(PipeTemp);
  json += "\",\"PF\":\"" + String(PipeTempVolF);
  json += "\",\"PS\":\"" + String(PipeTempVolS);
  json += "\",\"TT\":\"" + String(TankTemp);
  json += "\",\"TF\":\"" + String(TankTempVolF);
  json += "\",\"TS\":\"" + String(TankTempVolS);
  json += "\",\"WT\":\"" + String(WaterTemp);
  json += "\",\"SSS\":\"" + String(SetSteamTempVolS);
  
  json += "\",\"HS\":\"" + String(heating_rate_steam);
  json += "\",\"HP\":\"" + String(heating_rate_pipe);
  json += "\",\"HT\":\"" + String(heating_rate_tank);

  json += "\",\"STS\":\"" + String(set_temp_steam);
  json += "\",\"STP\":\"" + String(set_temp_pipe);
  json += "\",\"AP\":\"" + String(atm_pressure);
  json += "\",\"AT\":\"" + String(air_temp);
  json += "\",\"AS\":\"" + String(auto_status);
  json += "\",\"FM\":\"" + String(free_mem);
  // SETTINGS
  json += "\",\"MIT\":\"" + String(min_hot_temp);
  json += "\",\"MST\":\"" + String(max_steam_temp);
  json += "\",\"MTT\":\"" + String(max_tank_temp);
  json += "\",\"HR\":\"" + String(heating_rate);
  json += "\",\"VS\":\"" + String();
  if (readValve() == true)
    json += " ОТКРЫТ ";
  else
    json += " ЗАКРЫТ ";
  json += "\"}";                     // не забудем закрыть фигурную скобку!
  HTTP.send(200, "text/json", json); // передаём json
}
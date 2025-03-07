#include "header.h"

void HTTP_Init(void) // функция инициализации HTTP
{
 FS_init(); // Включаем работу с файловой системой

  //Выполнение команды из браузера
  HTTP.on("/button", handleButton); // обрашение к кнопкам через web интерфейс
  HTTP.on("/SetForm", handleSetForm);
  HTTP.on("/DelS", handleDeltaSteam); // обрашение к уставке steam_temp через web интерфейс
  HTTP.on("/DelP", handleDeltaPipe);  // обрашение к уставке pipe_temp(2/3) через web интерфейс
  HTTP.on("/data.json", handleData);   // формирование json файла для передачи данных в web интерфейс
  HTTP.begin();                        // Запускаем HTTP сервер
}

void handleSetForm()
{ // функция изменения настроек с web страницы

  autosam_mode = HTTP.arg("autosam_mode_h").toInt(); // получаем от клиента строку с режимом
  EEPROM_Write(autosam_mode_addr, autosam_mode);

  min_hot_temp = HTTP.arg("min_hot_temp_h").toFloat();
  EEPROM_Write(min_hot_temp_addr, min_hot_temp);

  max_tank_temp = HTTP.arg("max_tank_temp_h").toFloat();
  EEPROM_Write(max_tank_temp_addr, max_tank_temp);

  max_steam_temp = HTTP.arg("max_steam_temp_h").toFloat();
  EEPROM_Write(max_steam_temp_addr, max_steam_temp);

  heating_rate = HTTP.arg("heating_rate_h").toFloat();
  EEPROM_Write(heating_rate_addr, heating_rate);

  // press_correction = HTTP.arg("press_correction").toInt();
  // EEPROM_write(press_corr_addr, press_correction);

  HTTP.send(200, "text/plain", "OK"); // передаём ответ
  EEPROM.commit();
}

//***************************************************************************************************
void handleButton()
{                                               // функция управления клапаном с web страницы
  int button_state = HTTP.arg("state").toInt(); // получаем от клиента строку с номером нажатой кнопки

  if (button_state == 6) // если передан номер кнопки 6 "Открыть"
  {
    valve_auto_mode = false;
    digitalWrite(valve_pin, ON);
    auto_status = "Opened, Man";
    beep();
  }
  if (button_state == 7) // если передан номер кнопки 7 "Закрыть"
  {
    valve_auto_mode = false;
    digitalWrite(valve_pin, OFF); // выключаем клапан
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
void handleDeltaSteam()
{                                                    // функция изменения уставок с web страницы //steam_temp
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
    set_temp_steam = steam_temp + delta_steam; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта
  valve_auto_mode = true;
  telnet.print("DeltaSteam="); // выводим новое значение уставки на UART
  telnet.println(delta_steam);
  telnet.print("set_temp_steam="); // выводим новое значение уставки на UART
  telnet.println(set_temp_steam);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//**************************************************************************************************
void handleDeltaPipe()
{                                                   // функция изменения уставок с web страницы //pipe_temp(2/3)
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
    set_temp_pipe = pipe_temp + delta_pipe; // устанавливаем температуру отключения клапана равной текущей температуре плюс дельта
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
  json += "\",\"ST\":\"" + String(steam_temp);
  json += "\",\"SF\":\"" + String(steam_temp_alc_fl);
  json += "\",\"SS\":\"" + String(steam_temp_alc_st);
  json += "\",\"PT\":\"" + String(pipe_temp);
  json += "\",\"PF\":\"" + String(pipe_temp_alc_fl);
  json += "\",\"PS\":\"" + String(pipe_temp_alc_st);
  json += "\",\"TT\":\"" + String(tank_temp);
  json += "\",\"TF\":\"" + String(tank_temp_alc_fl);
  json += "\",\"TS\":\"" + String(tank_temp_alc_st);
  json += "\",\"WT\":\"" + String(water_temp);
  json += "\",\"SSS\":\"" + String(set_steam_temp_alc_st);
  
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
  json += "\",\"VS\":\"" + String(readValve());
 
   json += "\"}";                     // не забудем закрыть фигурную скобку!
  HTTP.send(200, "text/json", json); // передаём json
}
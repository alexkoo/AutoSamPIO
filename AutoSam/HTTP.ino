#include "header.h"

void HTTP_Init(void) // функция инициализации HTTP
{
  FS_init(); // Включаем работу с файловой системой

  // Выполнение команды из браузера

  HTTP.on("/button", handleButton);
  HTTP.on("/SetForm", handleSetForm);
  HTTP.on("/SetFormMode", handleSetFormMode);
  HTTP.on("/SetFormIndex", handleSetFormIndex);
  HTTP.on("/DelS", handleDeltaSteam);
  HTTP.on("/DelP", handleDeltaPipe);
  HTTP.on("/data.json", handleData);
  // HTTP.on("/dataset.json", handleDataSet); // формирование json файла для передачи данных в web интерфейс
  HTTP.begin(); // Запускаем HTTP сервер
}

// получение индекса режима
void handleSetFormMode()
{
  autosam_mode = HTTP.arg("autosam_mode_h").toInt();
  EEPROM.put(autosam_mode_addr, autosam_mode);
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

// получение индекса датчика
void handleSetFormIndex()
{
  ds_index = HTTP.arg("ds_index").toInt();
  saveDS();
  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

// получение настроек с web страницы
void handleSetForm()
{

  min_hot_temp = HTTP.arg("min_hot_temp_h").toFloat();
  EEPROM.put(min_hot_temp_addr, min_hot_temp);

  max_tank_temp = HTTP.arg("max_tank_temp_h").toFloat();
  EEPROM.put(max_tank_temp_addr, max_tank_temp);

  max_steam_temp = HTTP.arg("max_steam_temp_h").toFloat();
  EEPROM.put(max_steam_temp_addr, max_steam_temp);

  heating_rate = HTTP.arg("heating_rate_h").toFloat();
  EEPROM.put(heating_rate_addr, heating_rate);

  press_correction = HTTP.arg("press_correction").toInt();
  EEPROM.put(press_corr_addr, press_correction);

  steam_corr = HTTP.arg("steam_correction").toFloat();
  EEPROM.put(steam_corr_addr, steam_corr);

  pipe_corr = HTTP.arg("pipe_correction").toFloat();
  EEPROM.put(pipe_corr_addr, pipe_corr);

  tank_corr = HTTP.arg("tank_correction").toFloat();
  EEPROM.put(tank_corr_addr, tank_corr);

  water_corr = HTTP.arg("water_correction").toFloat();
  EEPROM.put(water_corr_addr, water_corr);

  HTTP.send(200, "text/plain", "OK"); // передаём ответ
  EEPROM.commit();
}

//   ***************************************************************************************************

// обработка кнопок с web страницы
void handleButton()
{
  int button_state = HTTP.arg("state").toInt(); // получаем от клиента строку с номером нажатой кнопки

  if (button_state == 6) // если передан номер кнопки 6 "Открыть"
  {
    valve_auto_mode = false;
    digitalWrite(valve_pin, ON);
    status_auto = 0;
    status_valve = 1;

    beep();
  }
  if (button_state == 7) // если передан номер кнопки 7 "Закрыть"
  {
    valve_auto_mode = false;
    digitalWrite(valve_pin, OFF); // выключаем клапан
    status_auto = 0;
    status_valve = 0;
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
  if (button_state == 43) // если передан номер кнопки 43 "EEPROM_Reset"
  {
    EEPROM_Reset();
    ESP.restart();
  }

  HTTP.send(200, "text/plain", "OK"); // передаём ответ
}

//**************************************************************************************************

// функция изменения уставок с web страницы //steam_temp
void handleDeltaSteam()
{
  float delta_steam = HTTP.arg("delta_s").toFloat(); // получаем от клиента строку с дельтой
  delay_steam = HTTP.arg("delay_s").toInt();         // получаем от клиента строку с задержкой
  if (delta_steam == 0)
  {
    set_temp_steam = 0;     // устанавливаем уставку 0
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

// функция изменения уставок с web страницы //pipe_temp(2/3)
void handleDeltaPipe()
{
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

// формирование json файла для передачи данных в web интерфейс
void handleData() // функция передачи файла data.json клиенту
{
  String json = "{";                                // начинаем с открывающейся фигурной скобки
  // head
  json += "\"TIME\":\"" + String(CurrentTime());    // кавычки экранируются!
  json += "\",\"RTIM\":\"" + String(millis2time()); // только 2 символа
  json += "\",\"VER\":\"" + String(VER);
  // json += "\",\"MOD\":\"" + String(autosam_mode);
  // index
  json += "\",\"ST\":\"" + String(steam_temp);
  json += "\",\"SF\":\"" + String(steam_temp_alc_fl);
  json += "\",\"SS\":\"" + String(steam_temp_alc_st);
  json += "\",\"PT\":\"" + String(pipe_temp);
  json += "\",\"PF\":\"" + String(pipe_temp_alc_fl);
  json += "\",\"PS\":\"" + String(pipe_temp_alc_st);
  json += "\",\"TT\":\"" + String(tank_temp);
  json += "\",\"TF\":\"" + String(tank_temp_alc_fl);
  json += "\",\"TS\":\"" + String(tank_temp_alc_st);
  json += "\",\"WT\":\"" + String(water_temp, 1);
  json += "\",\"SSS\":\"" + String(set_steam_temp_alc_st);
  json += "\",\"HS\":\"" + String(heating_rate_steam);
  json += "\",\"HP\":\"" + String(heating_rate_pipe);
  json += "\",\"HT\":\"" + String(heating_rate_tank);
  json += "\",\"STS\":\"" + String(set_temp_steam);
  json += "\",\"STP\":\"" + String(set_temp_pipe);
  json += "\",\"AP\":\"" + String(atm_pressure, 1);
  json += "\",\"AT\":\"" + String(air_temp, 1);
  json += "\",\"AS\":\"" + String(auto_status);
  // json += "\",\"FM\":\"" + String(free_mem);
  // json += "\",\"VS\":\"" + String(readValve());

  //  SETTINGS
  json += "\",\"MIT\":\"" + String(min_hot_temp, 1);
  json += "\",\"MST\":\"" + String(max_steam_temp, 1);
  json += "\",\"MTT\":\"" + String(max_tank_temp, 1);
  json += "\",\"HR\":\"" + String(heating_rate, 1);

  json += "\",\"SC\":\"" + String(steam_corr);
  json += "\",\"PC\":\"" + String(pipe_corr);
  json += "\",\"TC\":\"" + String(tank_corr);
  json += "\",\"WC\":\"" + String(water_corr);
  json += "\",\"ADDR\":\"" + String(addr_str);

  json += "\"}";                     // не забудем закрыть фигурную скобку!
  HTTP.send(200, "text/json", json); // передаём json
}




#include "header.h"

void clok()
{                                             // функция получения текущего времени с NTP сервера
  cur_ms = millis();                          // текущее количество миллисекунд
  t_cur = cur_ms / 1000;                      // текущее количество секунд
  if (cur_ms < ms2 || (cur_ms - ms2) > 60000) // Каждые 60 секунд считываем время в интернете
  {
    err_count++; // отсчитываем попытку синхронизации
    // делаем три попытки синхронизации с интернетом
    if (GetNTP()) // если попытка увенчалась успехом
    {
      ms2 = cur_ms;  // перезагружаем таймер на 60 секунд
      err_count = 0; // обнуляем счётчик попыток
      t_correct = ntp_time - t_cur;
    } // вычисляем поправку
  }
  if (cur_ms < ms1 || (cur_ms - ms1) > 500) // каждые 0.5 секунды выдаем время
  {
    ms1 = cur_ms;                 // перезагружаем таймер на 0.5 секунды
    ntp_time = t_cur + t_correct; // вычисляем время NTP
    //DisplayTime();                                                       // выводим текущее время на UART
    points = !points;
  } // инвертируем
}
//*************************************************************************************************************************************
void DisplayTime(void) // функция вывода времени на UART
{
  uint16_t m = (ntp_time / 60) % 60;
  uint16_t h = (ntp_time / 3600) % 24;
  Serial.print(h);
  Serial.print(":");
  Serial.println(m);
}
//*************************************************************************************************************************************
bool GetNTP(void) // функция посылки запроса к NTP серверу и парсинг ответа
{
  WiFi.hostByName(ntpServerName, timeServerIP);
  sendNTPpacket(timeServerIP); // посылаем запрос на NTP сервер
  delay(1000);
  int cb = udp.parsePacket();
  if (!cb)
  {
    if (debug == 2)
    {
      Serial.println("No packet yet");
    }
    return false;
  }
  else
  {
    Serial.print("packet received, length=");
    Serial.println(cb);
    udp.read(packetBuffer, NTP_PACKET_SIZE);                           // читаем пакет в буфер
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]); // 4 байта начиная с 40-го содержат таймстамп -
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  // число секунд начиная с 01.01.1900
    unsigned long secsSince1900 = highWord << 16 | lowWord;            // конвертируем два слова в переменную long
    const unsigned long seventyYears = 2208988800UL;                   // конвертируем в UNIX-таймстамп (число секунд от 01.01.1970)
    unsigned long epoch = secsSince1900 - seventyYears;
    ntp_time = epoch + TIMEZONE * 3600; // делаем поправку на местную тайм-зону
    Serial.print("Unix time = ");
    Serial.println(ntp_time);
  }
  return true;
}
//************************************************************************************************************************************
unsigned long sendNTPpacket(IPAddress &address) // функция посылки запроса NTP серверу на заданный адрес
{
  Serial.println("sending NTP packet...");
  memset(packetBuffer, 0, NTP_PACKET_SIZE); // очистка буфера в 0
  // Формируем строку запроса NTP сервера
  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[1] = 0;          // Stratum, or type of clock
  packetBuffer[2] = 6;          // Polling Interval
  packetBuffer[3] = 0xEC;       // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // Посылаем запрос на NTP сервер (123 порт)
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
  return 0;                   //fix??
}
//**********************************************************************************************************************************
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
//***********************************************************************************************************************************
String CurrentTime(void) // функция формирования строки "текущее время"
{
  String Time = "";                // начинаем с пустой строки
  byte m = (ntp_time / 60) % 60;   // вычисляем количество минут
  byte h = (ntp_time / 3600) % 24; // вычисляем количество часов
  if (h < 10)
    Time += "0";           // добавляем незначащий ноль к часам
  Time += (String)h + ":"; // добавляем часы и двоеточие
  if (m < 10)
    Time += "0";     // добавляем незначащий ноль к минутам
  Time += (String)m; // добавляем минуты
  return Time;
} // функция возвращает строку

// Инициализация FFS
void FS_init(void)
{
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
    }
  }

  //HTTP страницы для работы с FFS
  // листинг директории
  HTTP.on("/list", HTTP_GET, handleFileList);
  // загрузка редактора editor
  HTTP.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm"))
      HTTP.send(404, "text/plain", "FileNotFound");
  });
  //Создание файла
  HTTP.on("/edit", HTTP_PUT, handleFileCreate);
  //Удаление файла
  HTTP.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  HTTP.on(
      "/edit", HTTP_POST, []() {
        HTTP.send(200, "text/plain", "");
      },
      handleFileUpload);

  //called when the url is not defined here
  //use it to load content from SPIFFS
  HTTP.onNotFound([]() {
    if (!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "FileNotFound");
  });
}

// Здесь функции для работы с файловой системой
String getContentType(String filename)
{
  if (HTTP.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path)
{

  if (path.endsWith("/"))
    path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = HTTP.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleFileUpload()
{
  if (HTTP.uri() != "/edit")
    return;
  HTTPUpload &upload = HTTP.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (fsUploadFile)
      fsUploadFile.close();
  }
}

void handleFileDelete()
{
  if (HTTP.args() == 0)
    return HTTP.send(500, "text/plain", "BAD ARGS");
  String path = HTTP.arg(0);
  if (path == "/")
    return HTTP.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return HTTP.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  HTTP.send(200, "text/plain", "");
  path = String();
}
void handleFileCreate()
{
  if (HTTP.args() == 0)
    return HTTP.send(500, "text/plain", "BAD ARGS");
  String path = HTTP.arg(0);
  if (path == "/")
    return HTTP.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return HTTP.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return HTTP.send(500, "text/plain", "CREATE FAILED");
  HTTP.send(200, "text/plain", "");
  path = String();
}
void handleFileList()
{

  if (!HTTP.hasArg("dir"))
  {
    HTTP.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = HTTP.arg("dir");
  Dir dir = SPIFFS.openDir(path);
  path = String();
  String output = "[";
  while (dir.next())
  {
    File entry = dir.openFile("r");
    if (output != "[")
      output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  output += "]";

  HTTP.send(200, "text/json", output);
}

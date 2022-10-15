# 1 "C:\\Users\\aakoz\\AppData\\Local\\Temp\\tmpv5co2muh"
#include <Arduino.h>
# 1 "C:/Users/aakoz/Documents/GitHub/AutoSamPIO/AutoSam/AutoSam.ino"

#include "header.h"
void setup();
void loop();
void HTTP_init(void);
void handle_SetForm();
void handle_Button();
void handle_DeltaSteam();
void handle_DeltaPipe();
void handleData();
void clok();
void DisplayTime(void);
bool GetNTP(void);
unsigned long sendNTPpacket(IPAddress &address);
String millis2time();
String CurrentTime(void);
void FS_init(void);
String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();
#line 4 "C:/Users/aakoz/Documents/GitHub/AutoSamPIO/AutoSam/AutoSam.ino"
void setup()
{
  setup0();
}
void loop()
{
  loop0();
}
# 1 "C:/Users/aakoz/Documents/GitHub/AutoSamPIO/AutoSam/HTTP.ino"
#include "header.h"

void HTTP_init(void)
{
  FS_init();


  HTTP.on("/button", handle_Button);
  HTTP.on("/SetForm", handle_SetForm);
  HTTP.on("/DelS", handle_DeltaSteam);
  HTTP.on("/DelP", handle_DeltaPipe);
  HTTP.on("/data.json", handleData);
  HTTP.begin();
}

void handle_SetForm()
{

  autosam_mode = HTTP.arg("autosam_mode_h").toInt();
  EEPROM_write(autosam_mode_addr, autosam_mode);

  min_hot_temp = HTTP.arg("min_hot_temp_h").toFloat();
  EEPROM_write(min_hot_temp_addr, min_hot_temp);

  max_tank_temp = HTTP.arg("max_tank_temp_h").toFloat();
  EEPROM_write(max_tank_temp_addr, max_tank_temp);

  max_steam_temp = HTTP.arg("max_steam_temp_h").toFloat();
  EEPROM_write(max_steam_temp_addr, max_steam_temp);

  heating_rate = HTTP.arg("heating_rate_h").toFloat();
  EEPROM_write(heating_rate_addr, heating_rate);




  HTTP.send(200, "text/plain", "OK");
  EEPROM.commit();
}


void handle_Button()
{
  int button_state = HTTP.arg("state").toInt();

  if (button_state == 6)
  {
    valve_auto_mode = false;
    digitalWrite(valve, ON);
    auto_status = "Opened, Man";
    beep();
  }
  if (button_state == 7)
  {
    valve_auto_mode = false;
    digitalWrite(valve, OFF);
    auto_status = "Closed, Man";
    beep();
  }
  if (button_state == 41)
  {
    findDS();
  }
  if (button_state == 42)
  {
    ESP.restart();
  }

  HTTP.send(200, "text/plain", "OK");
}


void handle_DeltaSteam()
{
  float delta_steam = HTTP.arg("delta_s").toFloat();
  delay_steam = HTTP.arg("delay_s").toInt();
  if (delta_steam == 0)
  {
    set_temp_steam = 0;
     if (set_temp_pipe == 0)
    {
      valve_auto_mode = false;
    }
  }
  else
    set_temp_steam = SteamTemp + delta_steam;
  valve_auto_mode = true;
  telnet.print("DeltaSteam=");
  telnet.println(delta_steam);
  telnet.print("set_temp_steam=");
  telnet.println(set_temp_steam);
  HTTP.send(200, "text/plain", "OK");
}


void handle_DeltaPipe()
{
  float delta_pipe = HTTP.arg("delta_p").toFloat();
  delay_pipe = HTTP.arg("delay_p").toInt();
  if (delta_pipe == 0)
  {
    set_temp_pipe = 0;
    if (set_temp_steam == 0)
    {
      valve_auto_mode = false;
    }
  }

  else
    set_temp_pipe = PipeTemp + delta_pipe;
  valve_auto_mode = true;
  telnet.print("Delta pipe=");
  telnet.println(delta_pipe);
  telnet.print("delay_pipe=");
  telnet.println(delay_pipe);
  telnet.print("set_temp_pipe=");
  telnet.println(set_temp_pipe);
  HTTP.send(200, "text/plain", "OK");
}


void handleData()
{
  String json = "{";
  json += "\"TIME\":\"" + String(CurrentTime());
  json += "\",\"RTIM\":\"" + String(millis2time());
  json += "\",\"VER\":\"" + String(VER);
  json += "\",\"MOD\":\"" + String(autosam_mode);

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

  json += "\",\"MIT\":\"" + String(min_hot_temp);
  json += "\",\"MST\":\"" + String(max_steam_temp);
  json += "\",\"MTT\":\"" + String(max_tank_temp);
  json += "\",\"HR\":\"" + String(heating_rate);
  json += "\",\"VS\":\"" + String(readValve());






  json += "\"}";
  HTTP.send(200, "text/json", json);
}
# 1 "C:/Users/aakoz/Documents/GitHub/AutoSamPIO/AutoSam/time_fs.ino"

#include "header.h"

void clok()
{
  cur_ms = millis();
  t_cur = cur_ms / 1000;
  if (cur_ms < ms2 || (cur_ms - ms2) > 600000)
  {
    err_count++;

    if (GetNTP())
    {
      ms2 = cur_ms;
      err_count = 0;
      t_correct = ntp_time - t_cur;
    }
  }
  if (cur_ms < ms1 || (cur_ms - ms1) > 500)
  {
    ms1 = cur_ms;
    ntp_time = t_cur + t_correct;

    points = !points;
  }
}

void DisplayTime(void)
{
  uint16_t m = (ntp_time / 60) % 60;
  uint16_t h = (ntp_time / 3600) % 24;
  Serial.print(h);
  Serial.print(":");
  Serial.println(m);
}

bool GetNTP(void)
{
  WiFi.hostByName(ntpServerName, timeServerIP);
  sendNTPpacket(timeServerIP);
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
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    ntp_time = epoch + TIMEZONE * 3600;
    Serial.print("Unix time = ");
    Serial.println(ntp_time);
  }
  return true;
}

unsigned long sendNTPpacket(IPAddress &address)
{
  Serial.println("sending NTP packet...");
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;

  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
  return udp;
}

String millis2time()
{
  String Time = "";
  unsigned long ss;
  byte mm, hh;
  ss = millis() / 1000;
  hh = ss / 3600;
  mm = (ss - hh * 3600) / 60;
  ss = (ss - hh * 3600) - mm * 60;
  if (hh < 10)
    Time += "0";
  Time += (String)hh + ":";
  if (mm < 10)
    Time += "0";
  Time += (String)mm + ":";
  if (ss < 10)
    Time += "0";
  Time += (String)ss;
  return Time;
}

String CurrentTime(void)
{
  String Time = "";
  byte m = (ntp_time / 60) % 60;
  byte h = (ntp_time / 3600) % 24;
  if (h < 10)
    Time += "0";
  Time += (String)h + ":";
  if (m < 10)
    Time += "0";
  Time += (String)m;
  return Time;
}


void FS_init(void)
{
  LittleFS.begin();
  {
    Dir dir = LittleFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
    }
  }



  HTTP.on("/list", HTTP_GET, handleFileList);

  HTTP.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm"))
      HTTP.send(404, "text/plain", "FileNotFound");
  });

  HTTP.on("/edit", HTTP_PUT, handleFileCreate);

  HTTP.on("/edit", HTTP_DELETE, handleFileDelete);


  HTTP.on(
      "/edit", HTTP_POST, []() {
        HTTP.send(200, "text/plain", "");
      },
      handleFileUpload);



  HTTP.onNotFound([]() {
    if (!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "FileNotFound");
  });
}


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
  if (LittleFS.exists(pathWithGz) || LittleFS.exists(path))
  {
    if (LittleFS.exists(pathWithGz))
      path += ".gz";
    File file = LittleFS.open(path, "r");
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
    fsUploadFile = LittleFS.open(filename, "w");
    filename = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {

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
  if (!LittleFS.exists(path))
    return HTTP.send(404, "text/plain", "FileNotFound");
  LittleFS.remove(path);
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
  if (LittleFS.exists(path))
    return HTTP.send(500, "text/plain", "FILE EXISTS");
  File file = LittleFS.open(path, "w");
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
  Dir dir = LittleFS.openDir(path);
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
# 1 "C:/Users/aakoz/Documents/GitHub/AutoSamPIO/AutoSam/unused.ino"
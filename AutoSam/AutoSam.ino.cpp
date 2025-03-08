# 1 "C:\\Users\\aakoz\\AppData\\Local\\Temp\\tmpxld7vr1z"
#include <Arduino.h>
# 1 "D:/GIT/AutoSamPIO/AutoSam/AutoSam.ino"

#include "header.h"
void setup();
void loop();
void HTTP_Init(void);
void handleSetForm();
void handleButton();
void handleDeltaSteam();
void handleDeltaPipe();
void handleData();
void telnetLoop();
void FS_init(void);
String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();
void handleFileDelete();
void handleFileCreate();
void handleFileList();
#line 4 "D:/GIT/AutoSamPIO/AutoSam/AutoSam.ino"
void setup()
{
  setup0();
}
void loop()
{
  loop0();
}
# 1 "D:/GIT/AutoSamPIO/AutoSam/HTTP.ino"
#include "header.h"

void HTTP_Init(void)
{
 FS_init();


  HTTP.on("/button", handleButton);
  HTTP.on("/SetForm", handleSetForm);
  HTTP.on("/DelS", handleDeltaSteam);
  HTTP.on("/DelP", handleDeltaPipe);
  HTTP.on("/data.json", handleData);
  HTTP.begin();
}

void handleSetForm()
{

  autosam_mode = HTTP.arg("autosam_mode_h").toInt();
  EEPROM_Write(autosam_mode_addr, autosam_mode);

  min_hot_temp = HTTP.arg("min_hot_temp_h").toFloat();
  EEPROM_Write(min_hot_temp_addr, min_hot_temp);

  max_tank_temp = HTTP.arg("max_tank_temp_h").toFloat();
  EEPROM_Write(max_tank_temp_addr, max_tank_temp);

  max_steam_temp = HTTP.arg("max_steam_temp_h").toFloat();
  EEPROM_Write(max_steam_temp_addr, max_steam_temp);

  heating_rate = HTTP.arg("heating_rate_h").toFloat();
  EEPROM_Write(heating_rate_addr, heating_rate);




  HTTP.send(200, "text/plain", "OK");
  EEPROM.commit();
}


void handleButton()
{
  int button_state = HTTP.arg("state").toInt();

  if (button_state == 6)
  {
    valve_auto_mode = false;
    digitalWrite(valve_pin, ON);
    status_auto = 0;
    status_valve = 1;

    beep();
  }
  if (button_state == 7)
  {
    valve_auto_mode = false;
    digitalWrite(valve_pin, OFF);
    status_auto = 0;
    status_valve = 0;
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


void handleDeltaSteam()
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
    set_temp_steam = steam_temp + delta_steam;
  valve_auto_mode = true;
  telnet.print("DeltaSteam=");
  telnet.println(delta_steam);
  telnet.print("set_temp_steam=");
  telnet.println(set_temp_steam);
  HTTP.send(200, "text/plain", "OK");
}


void handleDeltaPipe()
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
    set_temp_pipe = pipe_temp + delta_pipe;
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
  json += "\",\"AP\":\"" + String(atm_pressure, 1);
  json += "\",\"AT\":\"" + String(air_temp, 1);
  json += "\",\"AS\":\"" + String(auto_status);


  json += "\",\"MIT\":\"" + String(min_hot_temp, 1);
  json += "\",\"MST\":\"" + String(max_steam_temp, 1);
  json += "\",\"MTT\":\"" + String(max_tank_temp, 1);
  json += "\",\"HR\":\"" + String(heating_rate, 0);

  json += "\",\"ADDR\":\"" + String(addr_str);
   json += "\"}";
  HTTP.send(200, "text/json", json);
}
# 1 "D:/GIT/AutoSamPIO/AutoSam/TIME_FS.ino"
#include "header.h"

void telnetLoop()
{
  if (telnetServer.hasClient())
  {
    if (!telnet || !telnet.connected())
    {
      if (telnet)
      {
        telnet.stop();
        Serial.println("Telnet Client Stop");
      }
      telnet = telnetServer.available();
      Serial.println("New Telnet client");
      telnet.print("Autosam telnet, debug mode: ");
      telnet.println(debug);
      telnet.flush();
    }
  }
  while (telnet.available())
  {
    Serial.write(telnet.read());
  }
}
# 155 "D:/GIT/AutoSamPIO/AutoSam/TIME_FS.ino"
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

  HTTP.on("/edit", HTTP_GET, []()
          {
    if (!handleFileRead("/edit.htm"))
      HTTP.send(404, "text/plain", "FileNotFound"); });

  HTTP.on("/edit", HTTP_PUT, handleFileCreate);

  HTTP.on("/edit", HTTP_DELETE, handleFileDelete);


  HTTP.on(
      "/edit", HTTP_POST, []()
      { HTTP.send(200, "text/plain", ""); },
      handleFileUpload);



  HTTP.onNotFound([]()
                  {
    if (!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "FileNotFound"); });
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
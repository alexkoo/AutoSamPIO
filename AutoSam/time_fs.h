#ifndef time_fs_h
#define time_fs_h

// Определяем переменные для процедур времени
long t_correct = 0;
unsigned long cur_ms = 0;
unsigned long ms1 = 0;
unsigned long ms2 = 10000000UL;
unsigned long t_cur = 0;
bool points = true;
unsigned int err_count = 0;    // счётчик ошибок
unsigned int localPort = 2390; // local port to listen for UDP packets
unsigned long ntp_time = 0;
#define TIMEZONE 3 // таймзона того места, где будет применяться устройство
// Для работы NTP
IPAddress timeServerIP;
const char *ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP udp;

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
void handleFileList();

#endif

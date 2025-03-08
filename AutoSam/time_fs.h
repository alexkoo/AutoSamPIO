#pragma once
#include <WString.h> // хз зачем

void telnetLoop(void);

void FS_init(void);

String getContentType(String filename);
bool handleFileRead(String);
void handleFileUpload();
void handleFileDelete();
void handleFileList();

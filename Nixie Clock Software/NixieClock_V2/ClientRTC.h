#include <Arduino.h>
#include "Settings.h"

const char* delim = " :";
char s[128];  // for sprintf

bool initRTC();
String getCurrentTime();
String getCurrentDate();
String print2digits(int number);
void setNewTimeRTC(time_t);
void setAlarmRTC();
String getCurrentDateTime();

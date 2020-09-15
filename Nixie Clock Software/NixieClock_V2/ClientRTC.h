#include <Arduino.h>
#include "Settings.h"

bool initRTC();
String getCurrentTime();
String getCurrentDate();
String print2digits(int number);
void setNewTimeRTC(unsigned long);
void setAlarmRTC();
String getCurrentDateTime();

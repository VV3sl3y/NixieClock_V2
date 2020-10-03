#include <Arduino.h>
#include "Settings.h"

bool initRTC();
String getCurrentTime();
String getCurrentDate();
String print2digits(int number);
void setNewTimeRTC(time_t);
void setAlarmRTC();
String getCurrentDateTime();

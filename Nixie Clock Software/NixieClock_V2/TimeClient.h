#pragma once

#include <Arduino.h>
#include "Settings.h"

extern int timeVar[6];
extern int dateVar[6];
extern int cathodeVar[6];
 
extern bool DateUpdated;
extern bool TimeUpdated;
extern bool RTCUpdated;
extern String currentDateESP;
extern String currentTimeESP;

#pragma region RTCFunctionality
bool initTimeClient();
String getCurrentTime();
String getCurrentDate();
String print2digits(int number);
void setNewTimeRTC(time_t);
void setAlarmRTC();
String getCurrentDateTime();
#pragma endregion

#pragma region TimeFunctionality
bool updateTimeVar();
bool updateDateVar();
time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss);
long stringToTime(String curDate, String curTime);
#pragma endregion
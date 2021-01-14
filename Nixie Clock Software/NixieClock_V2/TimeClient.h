#pragma once

#include <Arduino.h>

#include "Settings.h"

#include <Time.h>
#include <TimeLib.h>
#include <RTClock.h>

long lastMillisCheckedRTC;
long lastMillisPCP;
long lastMillisSwitchMode;
long lastMillisConnectionESP;
long lastMillisUpdatedESP;
long curMillis;

int cycleCurrent;
int FadeInNewMode;

int timeVar[6];
int dateVar[6];
int cathodeVar[NumberOfNixies];

bool DateUpdated;
bool TimeUpdated;
bool RTCUpdated;
String currentDateESP;
String currentTimeESP;

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
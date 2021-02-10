#pragma once

#include <Arduino.h>

//pre-defined responses
const String ESP_CONNECTED =  "ESPConnected";

//pre-defined commands
const String IS_ESP_CONNECTED =  "isConnectedESP";
const String GET_RSSI = "GetSignalStrength";
const String GET_DATE = "GetDate";
const String GET_TIME = "GetTime";
const String IS_DATA_UPDATE_AVAIABLE = "GetUpdateAvailable";
const String NO_DATA_RETURNED = "NoDataAvailable";

bool ConnectedESP;
bool MaxTriesHit;
String CurrentProcessingCommand;
int NumberOfConnectionTries;

bool initESP();
bool sendDataESP(String);
String receiveDataESP();
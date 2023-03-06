#pragma once

#include <Arduino.h>

/* ToDo ESP communicatie
 * Vraag tijdupdate
 * Vraag datumupdate
 * Settings.h variabelen
 **/

//pre-defined responses
const String ESP_CONNECTED				=  "ESPConnected";

//pre-defined commands
const String IS_ESP_CONNECTED			= "isConnectedESP";
const String GET_RSSI					= "GetSignalStrength";
const String GET_DATE					= "GetDate";
const String GET_TIME					= "GetTime";
const String IS_DATA_UPDATE_AVAIABLE	= "GetUpdateAvailable";
const String NO_DATA_RETURNED			= "NoDataAvailable";

extern bool ConnectedESP;
extern bool MaxTriesHit;
extern String CurrentProcessingCommand;
extern int NumberOfConnectionTries;

bool initESP();
bool sendDataESP(String);
String receiveDataESP();
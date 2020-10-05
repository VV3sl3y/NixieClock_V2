#include <Arduino.h>

//pre-defined responses
#define ESP_CONNECTED "ESPConnected"

//pre-defined commands
#define IS_ESP_CONNECTED "isConnectedESP"
#define GET_RSSI "GetSignalStrength"
#define GET_DATE "GetDate"
#define GET_TIME "GetTime"
#define IS_DATA_UPDATE_AVAIABLE "GetUpdateAvailable"
#define NO_DATA_RETURNED "NoDataAvailable"

bool initESP();
bool sendDataESP(String);
String receiveDataESP();
#include <Arduino.h>

//pre-defined responses
#define ESP_CONNECTED "ESPConnected"

//pre-defined commands
#define IS_ESP_CONNECTED "isConnectedESP"
#define GET_RSSI "GetSignalStrength"
#define GET_TIME "GetTime"
#define GET_DATE "GetDate"
#define GET_EPOCH "GetEpoch"

bool initESP();
bool checkConnectionESP();
String getEthernetTime();
String getEthernetDate();
String getEthernetEpoch();
int getSignalStrenght();
String receiveDataESP(String);

#include <Arduino.h>

//pre-defined responses
#define ESP_CONNECTED "ESPConnected"

//pre-defined commands
#define IS_ESP_CONNECTED "isConnectedESP"
#define GET_RSSI "GetSignalStrength"
#define GET_TIME "GetTime"
#define GET_DATE "GetDate"

bool initESP();
String getEthernetTime();
String getEthernetDate();
int getSignalStrenght();
String receiveDataESP(String);

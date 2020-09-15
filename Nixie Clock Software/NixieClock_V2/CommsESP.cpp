#include "CommsESP.h"
#include "Pinout.h"

#define DebugMode 2

//#include <SoftwareSerial.h>
//SoftwareSerial portESP(RX_ESP, TX_ESP);

#include "SoftSerialSTM32.h"    
SoftSerialSTM32 SWSerialESP(RX_ESP, TX_ESP); //mcuTX, mcuRX

bool initESP()
{
  SWSerialESP.begin(115200);

  SWSerialESP.listen();
  
  //activate ESP
  digitalWrite(Flash_ESP, HIGH);
  delay(50);
  digitalWrite(RST_ESP, HIGH);
  delay(50);

  delay(5000);
  
  //get garbage from STM start
  SWSerialESP.flush();
  delay(100);
  SWSerialESP.readString();
  delay(100);

  if(checkConnectionESP())
  {
    return true;
  }

  return false;
}

bool checkConnectionESP()
{
  #ifdef DebugMode
  if(DebugMode >= 3){
      Serial.println("now executing checkConnectionESP() ");
  }
  #endif
  String cmdResponse = receiveDataESP(IS_ESP_CONNECTED);
  if (cmdResponse == ESP_CONNECTED)
  {
    return false;
  }
  return true;
}

String getEthernetTime()
{
  #ifdef DebugMode
  if(DebugMode >= 3){
      Serial.println("now executing getEthernetTime() ");
  }
  #endif
  String tmpMessage = receiveDataESP(GET_TIME);
  if (tmpMessage == "" || tmpMessage ==  "Max tries hit!")
  {
    return "01:00:00";
  }
  else
  {
    return tmpMessage;
  }
}

String getEthernetDate()
{
  #ifdef DebugMode
  if(DebugMode >= 3){
      Serial.println("now executing getEthernetDate() ");
  }
  #endif
  String tmpMessage = receiveDataESP(GET_DATE);
  if (tmpMessage == "" || tmpMessage ==  "Max tries hit!")
  {
    return "01-01-20";
  }
  else
  {
    return tmpMessage;
  }
}

String getEthernetEpoch()
{
  #ifdef DebugMode
  if(DebugMode >= 3){
      Serial.println("now executing getEthernetEpoch() ");
  }
  #endif
  String tmpMessage = receiveDataESP(GET_EPOCH);
  if (tmpMessage == "" || tmpMessage ==  "Max tries hit!")
  {
    return "0";
  }
  else
  {
    return tmpMessage;
  }
}

int getSignalStrenght()
{
  #ifdef DebugMode
  if(DebugMode >= 3){
      Serial.println("now executing getSignalStrenght() ");
  }
  #endif
  if (receiveDataESP(GET_RSSI) == "")
  {

  }
  else
  {

  }
}

String receiveDataESP(String command)
{
  #ifdef DebugMode
  if(DebugMode >= 3){
      Serial.println("now executing receiveDataESP() ");
  }
  #endif
  String tmpMessage = "";
  int maxTries = 25;

  SWSerialESP.listen();
  delay(20);
  SWSerialESP.flush();
  SWSerialESP.print(command + ';');
  delay(50);
  
  while (maxTries >= 0)
  {
    if(SWSerialESP.available() > 0)
    {
      tmpMessage = SWSerialESP.readStringUntil(';');
      delay(30);
      #ifdef DebugMode
      if(DebugMode >= 2){
        Serial.println("Following message received from ESP: " + tmpMessage);
      }
      #endif
      return tmpMessage;
    }
    else
    {
      SWSerialESP.flush();
      SWSerialESP.println(command + ';');
    }

    SWSerialESP.flush();
    SWSerialESP.print(command + ';');
    delay(50);

    maxTries--;
    if (maxTries <= 0)
    {
      return "Max tries hit!";
    }
  }
  return tmpMessage;
}

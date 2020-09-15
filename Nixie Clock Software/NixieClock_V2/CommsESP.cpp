#include "CommsESP.h"
#include "Pinout.h"

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
  delay(5);
  digitalWrite(RST_ESP, HIGH);

  SWSerialESP.flush();
  
  delay(10000);

  //get garbage from STM start
  SWSerialESP.flush();
  SWSerialESP.print("Init" + ';');
  delay(50);
  String tmpMessage = SWSerialESP.readStringUntil(';');
  delay(50);

  String cmdResponse = receiveDataESP(IS_ESP_CONNECTED);
  if (cmdResponse != ESP_CONNECTED)
  {
    #ifdef DebugMode
    if(DebugMode >= 2){
      Serial.println("recvData returned: " + cmdResponse);
    }
    #endif
    return false;
  }
  return true;
}

String getEthernetTime()
{
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
  if (receiveDataESP(GET_RSSI) == "")
  {

  }
  else
  {

  }
}

String receiveDataESP(String command)
{
  String tmpMessage = "";
  int maxTries = 10;

  SWSerialESP.listen();
  delay(10);
  SWSerialESP.flush();
  SWSerialESP.print(command + ';');
  delay(50);
  
  while (maxTries >= 0)
  {
    if(SWSerialESP.available() > 0)
    {
      tmpMessage = SWSerialESP.readStringUntil(';');
      delay(30);
      Serial.println("Following message received from ESP: " + tmpMessage);
      return tmpMessage;
    }
    else
    {
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

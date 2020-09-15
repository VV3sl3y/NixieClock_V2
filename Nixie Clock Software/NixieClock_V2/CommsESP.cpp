#include "CommsESP.h"
#include "Pinout.h"
//#include <SoftwareSerial.h>

//SoftwareSerial portESP(TX_ESP, RX_ESP);

bool initESP()
{
  Serial1.begin(115200);
  
  //initialisation of the pins is done by the Pinout to minimize the setup time delay is to wait for propper connection
  delay(1000);

  if (receiveDataESP(IS_ESP_CONNECTED) != ESP_CONNECTED)
  {
    return false;
  }
  else
  {
    getEthernetTime();
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
    return "01:01:10";
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
  while (maxTries >= 0)
  {
    if (Serial1.available() > 0)
    {
      tmpMessage = Serial1.readStringUntil('\r\n');
      return tmpMessage;
    }
    else
    {
      Serial1.print(command + "\r\n");
    }
    delay(100);

    maxTries--;
    if (maxTries <= 0)
    {
      return "Max tries hit!";
    }
  }
  return tmpMessage;
}

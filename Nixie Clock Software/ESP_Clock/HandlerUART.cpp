#include "HandlerUART.h"

bool InitHandlerUART()
{
  
}

void HandleIncomingRequests()
{
  if (Serial.available() > 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    String command = Serial.readStringUntil(';');
    Serial.flush();
    delay(10);
    if(command == "isConnectedESP")
    {
      Serial.print("ESPConnected;");
    }
    else if(command == "GetTime")
    {
      String currentTime = timeClient.getFormattedTime();
      Serial.print(currentTime + ";");
    }
    else if(command == "GetDate")
    {
      String currentDate = timeClient.getFormattedDate();
      Serial.print(currentDate + ";");
    }
    else if(command == "GetSignalStrength")
    {
      int8_t rssi = 95;
      
      Serial.print("Signal Strength (RSSI): ");
      Serial.print(rssi);
      Serial.print("%;");
    }
    else
    {
      Serial.print(command + ";");
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
}

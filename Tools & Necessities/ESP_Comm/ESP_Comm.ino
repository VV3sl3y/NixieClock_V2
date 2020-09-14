
void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
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
        String currentTime = "123456";
        Serial.print(currentTime + ";");
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

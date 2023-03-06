

/*bool initEEPROM()
{
    EEPROM.begin(512);
}


byte readAddressEEPROM(int address)
{
  byte value = EEPROM.read(address);
  
  // there are only 512 bytes of EEPROM, from 0 to 511, so if we're
  // on address 512, error
  if (address >= 512) { address = 0; value = null; }

  return value;
}

bool writeDataToBufferEEPROM(int addr, int value)
{
  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  EEPROM.write(addr, value);
}

bool commitBufferToEEPROM()
{
  if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
      return true;
    } else {
      Serial.println("ERROR! EEPROM commit failed");
      return false;
    }
}

void clearEEPROM() {
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++) { EEPROM.write(i, 0); }

  // turn the LED on when we're done
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  EEPROM.end();
}*/

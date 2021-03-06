# NixieClock_V2
Repository for the Nixie Clock software based around a STM32 micro-controller and stackable PCB's

Private Repository for the upkeep of the Nixie Clock software for the V2 design PCB.

This project has dependencies for specific libraries to control different components.

The used libraries for the ESP are:
- ArduinoOTA.h
- ESP8266WiFi.h
- ESP8266WebServer.h
- ESP8266mDNS.h
- TimeLib
- WiFiManager

The used libraries for the STM are:
- STM32 library (STM32Duino.com)
- STM32 RTC library (RTCClock.h)
- FastLED (editted for STM32 bluepill)

http://arduino.esp8266.com/stable/package_esp8266com_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json

#TODO List
Nixieclock Hardware:
  - Stroom nameten, zoeken naar polyfuse(300-500mA?)
  - Knoppen zoeken(RST & Alarm(2x NO), flashknop = header(NC))
  - Batterijen en houders zoeken (2032)
  - Behuizing tekenen

Nixieclock software:
  - Afhandeling ESP update available
  - HV powerdown at set time
  - EEPROM aanmaken en settings ernaar schrijven
  - EEPROM init: eerste boot/full reset: parsebuildtimestamp()
  - opstarten aanpassen: ESP aanzetten, RTC tijd inladen en later aanpassen (flag voor ESP connected toevoegen met max retries)
  - pre-def LED animaties? (Static, Breathing, RGB, knight rider)

NixieESP software:
  - flag zetten wanneer er een update van de settings is geweest (onderscheid maken tussen welke settings zijn veranderd?)
  - Webpage voor instellingen aanvullen in esp (rgb leds, off time, alle huidige settings defines in stm)
  - WifiManager webhost mode als er geen wifi is

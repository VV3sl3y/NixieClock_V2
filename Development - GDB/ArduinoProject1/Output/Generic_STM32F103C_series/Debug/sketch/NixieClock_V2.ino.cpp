#include <Arduino.h>
#line 1 "P:\\VS_Sources\\GDB\\ArduinoProject1\\sketches\\NixieClock_V2.ino"
/*
----------------------------------------------------------------------------------------------------------
 * Software written for the NixieClock V2 and probably V2.1 will be included here too
 * Development is mostly done on the V2 board
 * Exception to this rule will be the communication with the ESP as RX/TX will be connected differently
 * and have yet to be tested on the V2.1
 * 
 * Programm start from the NixieCore. Mostly only the settingsfile is needed. When using internet on the
 * NixieClock the settings should also be available online
-----------------------------------------------------------------------------------------------------------
*/

/*
	Nieuwe indeling files:
	CommsESP
		communicatie met ESP
		flags zetten voor programma update
	
	DriveNixies
		afhandeling nixie states (cijfers & dots)
	
	NixieClockCore
		Soort van Threading control waar d.m.v. millis updates worden uitgevoerd om de klok "lagvrij" te laten lopen
		
	NixieLighting
		afhandeling fastled library
		animatie afhandeling leds
		
	Pinout
		pin definitie & initialisatie
		
	Runmodes
		clock modes waar de klok mee bezig is en wat die moet doen
	
		
	settings
		definities van de standaard waarden voor de klok
		EEPROM afhandeling (lezen & schrijven)

	TimeClient NOTE: offset from time RTC to realtime ~ 37 seconds
		opstart tijd
		RTC handling
		alarm handling
		verdere tijdafhandeling intern
*/

/*
TODO Lijst:
	fix de code van multiple definitions
	
	------------------------------------------------------------------------------------------------------------------------------------------------------
	- Flickering van eerste cijfer nakijken
	
	- Commando's met ESP nakijken dat deze zonder flickering verloopt(nu stopt de multiplexer merkbaar)
	
	-Runmodes nagaan(RunUpdateMode kan bijvoorbeeld al efficienter m.b.v. TimeClient functies)
	
	- Voor de tijdscheck bij opstarten :
		bij setup, vraag parsebuildtimestamp aan
		als huidige RTC tijd < parsebuildtimestamp : RTC tijd = parsebuildtimestamp ? RTC tijd = RTC tijd
		Bij ESP tijd zelfde : RTC tijd < ESP tijd : RTC tijd = ESP tijd ? RTC tijd = RTC tijd
		Originele RTCClock  code :
		  ParseBuildTimestamp(mtt);   // get the Unix epoch Time counted from 00:00:00 1 Jan 1970
		  tt = rtclock.makeTime(mtt) + 25;  // additional seconds to compensate build and upload delay
		  rtclock.setTime(tt);
	
	- Voor de powerdown van de nixies in de Drivenixies funties aanmaken, dit omvat dan de powerup en powerdown voor de nixies
	
	- FastLED, implementeer animaties in LEDS
	
	- De animatie van preventcathodepoisoning(PCP) smoothen om van klok stand naar datumstand te gaan gebruik de huidige notatie en stop de animatie wanneer het cijfer op de nieuwe staat
	
	- schakel de dots mee (meteen aan het begin) can PCP
	
	- Class aanmaken voor de afhandeling van het alarm, of bijvoegen in de RTC
	
	- Implementeer afvangen beschikbare data ESP(aanvraag data beschikbaar ??) voornamelijk in de ESP

	- Maak een update available voor esp vs stm

	- Voor de ESP commando's(aanpassen settings), vlag toevoegen welke bijhoud of er een commando bezig is om de tijdsweergave zo min mogelijk te beinvloeden
	
	- ESP baud speed hoger proberen dan dat deze nu staat
	
	- voor speedup de digitalreadwrite omzetten naar DigitalReadWriteFast voor definitieve versie software de digitalreadwrite omzetten naar portmodulatie
*/

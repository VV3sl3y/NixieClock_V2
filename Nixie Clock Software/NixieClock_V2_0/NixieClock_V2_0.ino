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
	DriveNixies
		afhandeling nixie states (cijfers & dots)

	ESPHandler
		communicatie met ESP

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

	TimeClient NOTE: offset from time RTC to realtime ~ 37 seconds, evt curmillis bij de ESP bij programmeren om dit te fixen
		opstart tijd
		RTC handling
		alarm handling
		verdere tijdafhandeling intern
*/

/*
TODO Lijst:
	fix de code van multiple definitions
    Alle definitions aanmaken als extern (in Header file waar het logisch is), vervolgens globaal in de bijbehorende cpp aanmaken als normale variabele
    vervolgens kan deze waarde probleemloos gebruikt worden in andere files :)
    https://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files/1433387

  - drie potentielen nakijken waarbij groot deel mogelijk herschreven moet worden
		- RTOS https://electronics.stackexchange.com/questions/346031/do-microcontrollers-support-threading
		- Port manipulation ipv seperate digitalwrites
		- Tasmota integratie op ESP

	------------------------------------------------------------------------------------------------------------------------------------------------------
  - Fout met het schakelen van HeaderBoard, loopt altijd op IN12?!?

	- Flickering van eerste cijfer nakijken

	- Commando's met ESP nakijken dat deze zonder flickering verloopt(nu stopt de multiplexer merkbaar)
			eventuele oplossing zou kunnen zijn dat de NixiesOff eerst wordt uitgevoerd voordat de tijd weer wordt weergegeven
			zo voorkomt het dat het eerste cijfer helderder oplicht (RunUpdateESPMode(), RunUpdateMode())

	- Voor de tijdscheck bij opstarten :
		bij setup, vraag parsebuildtimestamp aan
		als huidige RTC tijd < parsebuildtimestamp : RTC tijd = parsebuildtimestamp ? RTC tijd = RTC tijd
		Bij ESP tijd zelfde : RTC tijd < ESP tijd : RTC tijd = ESP tijd ? RTC tijd = RTC tijd
		Originele RTCClock  code :
		  ParseBuildTimestamp(mtt);   // get the Unix epoch Time counted from 00:00:00 1 Jan 1970
		  tt = rtclock.makeTime(mtt) + 25;  // additional seconds to compensate build and upload delay
		  rtclock.setTime(tt);

	- Voor de powerdown van de nixies in de Drivenixies funties aanmaken, dit omvat dan de powerup en powerdown voor de nixies, eventueel met settime

	- FastLED, implementeer animaties in LEDS (Static, Breathing, RGB, knight rider)

	- De animatie van preventcathodepoisoning(PCP) smoothen om van klok stand naar datumstand te gaan gebruik de huidige notatie en stop de animatie wanneer het cijfer op de nieuwe staat

	- schakel de dots mee (meteen aan het begin) can PCP

	- Class aanmaken voor de afhandeling van het alarm, of bijvoegen in de RTC

	- Implementeer afvangen beschikbare data ESP(aanvraag data beschikbaar ??) voornamelijk in de ESP

	- Maak een update available voor esp vs stm

	- Voor de ESP commando's(aanpassen settings), vlag toevoegen welke bijhoud of er een commando bezig is om de tijdsweergave zo min mogelijk te beinvloeden

	- ESP baud speed hoger proberen dan dat deze nu staat

	- voor speedup de digitalreadwrite omzetten naar DigitalReadWriteFast voor definitieve versie software de digitalreadwrite omzetten naar portmodulatie

	- EEPROM Implementatie: initialisatie(first upload eerste boot/full reset: parsebuildtimestamp()) & afhandeling


Nixieclock Hardware:
	- Stroom nameten, zoeken naar polyfuse(300-500mA?)

	- Behuizing tekenen

NixieESP software:
	  - flag zetten wanneer er een update van de settings is geweest (onderscheid maken tussen welke settings zijn veranderd?)

	  - Webpage voor instellingen aanvullen in esp (rgb leds, off time, alle huidige settings defines in stm)

	  - WifiManager webhost mode als er geen wifi is
*/

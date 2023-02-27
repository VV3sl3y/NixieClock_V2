# 1 "P:\\GitHub\\NixieClock_V2\\Development - GDB\\ArduinoProject1\\sketches\\NixieClock_V2_1.ino"
/*

----------------------------------------------------------------------------------------------------------

 * Software written for the NixieClock V2.1 Software version V1.0 [V2.1.1.0]

 * Development phased to Board V2.1 as the V2 is DEPRECATED

 *

 * Programm start from the NixieCore. Mostly only the settingsfile is needed. When using internet on the

 * NixieClock the settings should also be available online

-----------------------------------------------------------------------------------------------------------

*/
# 11 "P:\\GitHub\\NixieClock_V2\\Development - GDB\\ArduinoProject1\\sketches\\NixieClock_V2_1.ino"
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

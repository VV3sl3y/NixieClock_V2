#include <Arduino.h>

#include "DriveNixies.h"
#include "ESPHandler.h"
#include "NixieLighting.h"
#include "Pinout.h"
#include "RunModes.h"
#include "Settings.h"
#include "TimeClient.h"

#include <Time.h>
#include <TimeLib.h>

bool initRunmodes()
{
	lastMillisCheckedRTC = 0;
	lastMillisPCP = 0;
	lastMillisSwitchMode = 0;
	lastMillisConnectionESP = 0;
	lastMillisUpdatedESP = 0;
	curMillis = 0;
	
	cycleCurrent = 0;
	FadeInNewMode = 0;
	
	NewClockState = MODE_TIME;
	CurrentClockState = MODE_TIME;
	return true;
}

void RunDebugMode() {
	digitalWrite(HVON, HIGH);
	for (int x = 0; x < NumberOfNixies; x++)
	{
		NixiesOff();
		SetNixieTube(x);
		for (int i = 0; i < 10; i++)
		{
			SetNixieDriverVal(i);
			delay(250); // making sure the nixie numbers are readable when passing through all values
		}
		digitalWrite(LED_RTC, !digitalRead(LED_RTC));
		SwitchDot(DOT_SWITCH);
		//analogWrite(Buzzer, 127);

		//colorTest();
	}
}

void RunTimeMode() {
	curMillis = millis();
	if ((curMillis - lastMillisCheckedRTC) > DisplayTimeUpdateInterval || curMillis < 0)
	{
		updateTimeVar();

		lastMillisCheckedRTC = curMillis;

		#ifdef DebugMode
		Serial.println("curmillis:" + String(curMillis) + "   oldMillisTimeCheckedRTC:" + String(lastMillisCheckedRTC));
		#endif

		if ((timeVar[0] % 2) == 1)
		{
			SwitchDot(DOT_ON);
		}
		else
		{
			SwitchDot(DOT_OFF);
		}
	}

	digitalWrite(HVON, HIGH);
	for (int x = 0; x < NumberOfNixies; x++)
	{
		NixiesOff();
		delayMicroseconds(NIXIE_OFF_TIME);
		SetNixieDriverVal(timeVar[x]);
		SetNixieTube(x);

		delayMicroseconds(NIXIE_ON_TIME);
	}
}

void RunDateMode() {
	curMillis = millis();
	if ((curMillis - lastMillisCheckedRTC) > DisplayDateUpdateInterval || curMillis < 0)
	{
		updateDateVar();

		lastMillisCheckedRTC = curMillis;
		#ifdef DebugMode
		Serial.println("curmillis:" + String(curMillis) + "   oldMillisDateRTC:" + String(lastMillisCheckedRTC));
		#endif
	}

	digitalWrite(HVON, HIGH);
	for (int x = 0; x < NumberOfNixies; x++)
	{
		NixiesOff();
		delayMicroseconds(NIXIE_OFF_TIME);
		SetNixieDriverVal(dateVar[x]);
		SetNixieTube(x);

		delayMicroseconds(NIXIE_ON_TIME);
	}
	SwitchDot(DOT_ON);
}

void RunPreventionCathodePoisoning(CLOCK_MODE NewState) {
	curMillis = millis();
	if ((curMillis - lastMillisPCP) > PCP_INTERVAL || curMillis < 0) {
		lastMillisPCP = millis();
		switch (NewState)
		{
		case MODE_TIME:
			updateTimeVar();
			for (int i = 0; i < FadeInNewMode; i++) {
				cathodeVar[i] = timeVar[i];
			}
			break;
		case MODE_DATE:
			updateDateVar();
			for (int i = 0; i < FadeInNewMode; i++) {
				cathodeVar[i] = dateVar[i];
			}
			break;
			
		default:
			break;
		}
		
		switch (AnimationPCP)
		{
		case 1:
			for (int i = FadeInNewMode; i < NumberOfNixies; i++) {
				cathodeVar[i] = cathodeVar[i] + 1;
				if (cathodeVar[i] > 9) {
					cathodeVar[i] = 0;
				}
			}
			break;
		case 2:
			for (int i = FadeInNewMode; i < NumberOfNixies; i++) {
				cathodeVar[i] = cathodeVar[i] - 1;
				if (cathodeVar[i] < 0) {
					cathodeVar[i] = 9;
				}
			}
			break;

		case 3:
			for (int i = FadeInNewMode; i < NumberOfNixies; i++) {
				cathodeVar[i] = random(10);
			}
			break;
			
		default:
			#ifdef DebugMode
			Serial.println("Error while animating, set animation: " + AnimationPCP);
			#endif
			break;
		}

		if (cycleCurrent < cyclePCP) {
			cycleCurrent++;
		}
		else if (FadeInNewMode < NumberOfNixies) {
			FadeInNewMode++;
			cycleCurrent = 0;
		}
		else
		{
			#ifdef DebugMode
			Serial.println("entering new afterpcp state: " + String(ClockState));
			#endif
			cycleCurrent = 0;
			FadeInNewMode = 0;
			ClockState = NewClockState;
		}
			#ifdef DebugMode
			Serial.println("curmillis:" + String(curMillis) + "   oldMillisPCP:" + String(lastMillisPCP));
			#endif
	}
  
	digitalWrite(HVON, HIGH);
	for (int x = 0; x < NumberOfNixies; x++)
	{
		NixiesOff();
		delayMicroseconds(NIXIE_OFF_TIME);
		SetNixieDriverVal(cathodeVar[x]);
		SetNixieTube(x);

		delayMicroseconds(NIXIE_ON_TIME);
	}
}

void RunUpdateESPMode()
{
	String CommandResult = "";
	
	switch (ESP_State)
	{
	case ESP_FREE:
		sendDataESP(CurrentProcessingCommand);
		ESP_State = ESP_AWAITING_ANSWER;
		break;
		
	case ESP_AWAITING_ANSWER:
		if (CurrentProcessingCommand  == IS_ESP_CONNECTED) {
			CommandResult = receiveDataESP();
			if (CommandResult == ESP_CONNECTED)
			{
				ConnectedESP = true;
			}
			else
			{
				ConnectedESP = false;
				NumberOfConnectionTries++;
				if (NumberOfConnectionTries >= Max_ESPConnection_Retries)
				{
					MaxTriesHit = true;
				}
			}
		}
		else if (CurrentProcessingCommand  == GET_DATE) {
			CommandResult = receiveDataESP();
			if (CommandResult != "")
			{
				currentDateESP = CommandResult;
				DateUpdated = true;
			}
		}
		else if (CurrentProcessingCommand  == GET_TIME) {
			CommandResult = receiveDataESP();
			if (CommandResult != "")
			{
				currentTimeESP = CommandResult;
				TimeUpdated = true;
			}
		}
		else if (CurrentProcessingCommand  == GET_RSSI) {
			
		}
		else if (CurrentProcessingCommand  == NO_DATA_RETURNED) {
			
		}
		else {
			//laatste slimigheierrord inbouwen afhandeling commandos
		}
		ESP_State = ESP_FREE;  //temp dummy to get through the code
		break;
		
	case ESP_COMMAND_REDO:
		sendDataESP(CurrentProcessingCommand);
		break;
		
	default:
		break;
	}
	if (!RTCUpdated && DateUpdated && TimeUpdated) {
		setNewTimeRTC(stringToTime(currentDateESP, currentTimeESP));
		RTCUpdated = true;
	}
}

void RunUpdateMode()
{
		#ifdef DebugMode
		Serial.println("New ethernet time: " + currentTimeESP);
		Serial.println("New ethernet date: " + currentDateESP);
		#endif
		if (currentDateESP != "" && currentTimeESP != "") {
			#ifdef DebugMode
			Serial.println("Setting time!");
			#endif
			time_t newTime = stringToTime(currentDateESP, currentTimeESP);
			setNewTimeRTC(newTime);  //compensation for reading serial
	}
	if (updateTimeVar() && updateDateVar())
	{
		//success
	}
}

void RunErrorMode() {

}
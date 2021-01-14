#include  "RunModes.h"

bool initRunmodes()
{
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
		if (DebugMode >= 2)
		{
			Serial.println("curmillis:" + String(curMillis) + "   oldMillisTimeCheckedRTC:" + String(lastMillisCheckedRTC));
		}
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
		if (DebugMode >= 2)
		{
			Serial.println("curmillis:" + String(curMillis) + "   oldMillisDateRTC:" + String(lastMillisCheckedRTC));
		}
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

void RunPreventionCathodePoisoning(CLOCK_MODE State) {
	curMillis = millis();
	if ((curMillis - lastMillisPCP) > PCP_INTERVAL || curMillis < 0) {
		lastMillisPCP = millis();
		if (State == MODE_TIME) {
			updateTimeVar();
			for (int i = 0; i < FadeInNewMode; i++) {
				cathodeVar[i] = timeVar[i];
			}
		}
		else if (State == MODE_DATE) {
			updateDateVar();
			for (int i = 0; i < FadeInNewMode; i++) {
				cathodeVar[i] = dateVar[i];
			}
		}
    
		if (AnimationPCP == 1) {
			for (int i = FadeInNewMode; i < NumberOfNixies; i++) {
				cathodeVar[i] = cathodeVar[i] + 1;
				if (cathodeVar[i] > 9) {
					cathodeVar[i] = 0;
				}
			}
		}
		else if (AnimationPCP == 2) {
			for (int i = FadeInNewMode; i < NumberOfNixies; i++) {
				cathodeVar[i] = cathodeVar[i] - 1;
				if (cathodeVar[i] < 0) {
					cathodeVar[i] = 9;
				}
			}
		}

		else if (AnimationPCP == 3) {
			for (int i = FadeInNewMode; i < NumberOfNixies; i++) {
				cathodeVar[i] = random(10);
			}
		}

		else {
			Serial.println("Error while animating, set animation: " + AnimationPCP);
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
			Serial.println("entering new afterpcp state: " + String(ClockState));
			cycleCurrent = 0;
			FadeInNewMode = 0;
			ClockState = NewClockState;
		}
#ifdef DebugMode
		if (DebugMode >= 2)
		{
			Serial.println("curmillis:" + String(curMillis) + "   oldMillisPCP:" + String(lastMillisPCP));
		}
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
	if (DebugMode >= 1)
	{
		Serial.println("New ethernet time: " + currentTimeESP);
		Serial.println("New ethernet date: " + currentDateESP);
		if (currentDateESP != "" && currentTimeESP != "") {
			Serial.println("Setting time!");
			time_t newTime = stringToTime(currentDateESP, currentTimeESP);
			setNewTimeRTC(newTime);  //compensation for reading serial
		}
	}
#endif

	char tmpTime[8];
	currentTimeESP.toCharArray(tmpTime, 8);

	timeVar[0] = (int)tmpTime[7] - 48;
	timeVar[1] = (int)tmpTime[6] - 48;
	timeVar[2] = (int)tmpTime[4] - 48;
	timeVar[3] = (int)tmpTime[3] - 48;
	timeVar[4] = (int)tmpTime[1] - 48;
	timeVar[5] = (int)tmpTime[0] - 48;

	char tmpDate[8];
	currentTimeESP.toCharArray(tmpDate, 8);

	dateVar[0] = (int)tmpDate[7] - 48;
	dateVar[1] = (int)tmpDate[6] - 48;
	dateVar[2] = (int)tmpDate[4] - 48;
	dateVar[3] = (int)tmpDate[3] - 48;
	dateVar[4] = (int)tmpDate[1] - 48;
	dateVar[5] = (int)tmpDate[0] - 48;
}

void RunErrorMode() {

}
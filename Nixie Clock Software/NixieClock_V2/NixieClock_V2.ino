#include "Pinout.h"
#include "Settings.h"
#include "DriveNixies.h"
#include "ClientRTC.h"
#include "NixieLighting.h"
#include "CommsESP.h"
#include <Time.h>
#include <TimeLib.h>

//#define TODO_LIST

#ifdef TODO_LIST
ERROR prios:
  **************************  IDEE VOOR DEBUGGEN: sluit een ESP op de bluepill aan en debug stap voor stap. HV etc zijn niet nodig namelijk   ****************************************************
  - fix ESP Communicatie voor tijd
  - kijk of tijd nu gefixed is zonder tijdzone
  - fix ESP initialisatie (los van timer met timeouts en retry e.d.)
  - Maak een update available voor esp vs stm
  - bekijk notities en errors
  - uncomment define todolist voor meer
	
	Voor de tijdscheck bij opstarten:
	bij setup, vraag parsebuildtimestamp aan
	als huidige RTC tijd < parsebuildtimestamp: RTC tijd = parsebuildtimestamp ? RTC tijd = RTC tijd
	Bij ESP tijd zelfde: RTC tijd < ESP tijd: RTC tijd = ESP tijd ? RTC tijd = RTC tijd
	Originele RTCClock  code:
	  ParseBuildTimestamp(mtt);  // get the Unix epoch Time counted from 00:00:00 1 Jan 1970
	  tt = rtclock.makeTime(mtt) + 25; // additional seconds to compensate build and upload delay
	  rtclock.setTime(tt);

	voor de commando's, vlag toevoegen welke bijhoud of er een commando bezig is om de tijdsweergave zo min mogelijk te beinvloeden
#endif

enum CLOCK_MODE
{
	MODE_DEBUG,
	MODE_TIME,
	MODE_DATE,
	MODE_PCP,
	MODE_UPDATE_TIME,
	MODE_UPDATE_ESPDATA,
	MODE_ERROR
} ClockState;

enum ESP_MODE
{
	ESP_FREE,
	ESP_AWAITING_ANSWER,
	ESP_COMMAND_REDO
} ESP_State;

CLOCK_MODE NewClockState = MODE_TIME;
CLOCK_MODE CurrentClockState;

int lastMillisCheckedRTC = 0;
int lastMillisPCP = 0;
int lastMillisSwitchMode = 0;
int lastMillisConnectionESP = 0;
int lastMillisUpdatedESP = 0;
int curMillis = 0;

int cycleCurrent = 0;
int FadeInNewMode = 0;

int timeVar[6] = {};
int dateVar[6] = {};
int cathodeVar[6] = {};

bool ConnectedESP = false;
bool MaxTriesHit = false;
String CurrentProcessingCommand = "";
int NumberOfConnectionTries = 0;

bool DateUpdated = false;
bool TimeUpdated = false;
bool RTCUpdated = false;
String currentDateESP;
String currentTimeESP;

void setup() {
	//Free up PB3 & PB4
	afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  
	//Start Serial communication
	#ifdef DebugMode
	  if(DebugMode > 0)
	{
		Serial.begin(115200);
	}
  #endif
  
	//set pinmodes
	if(initPins()) {
		if (DebugMode >= 2)
		{
			Serial.println("Pins initialized");
		}
	} else {
		if (DebugMode >= 2)
		{
			Serial.println("Failed to initialize pins");
		}
	}

	//initialize WS2811B
	if(initLEDS()) {
	  #ifdef TODO_LIST
		ERROR FastLED, implementeer animaties in LEDS
		#endif
		setNeoColor(0, 230, 255);
		#ifdef DebugMode
		if (DebugMode >= 2)
		{
			Serial.println("FastLED initialized");
		}
		#endif
	} else {
	#ifdef DebugMode
		if (DebugMode >= 2)
		{
			Serial.println("Failed to initialize FastLED");
		}
		#endif
	}

	//initialize RTC
	if(initRTC()) {
		if (DebugMode >= 2)
		{
			Serial.println("RTC initialized");
		}
	} else {
		if (DebugMode >= 2)
		{
			Serial.println("Failed to initialize RTC");
		}
	}

	#ifdef TODO_LIST 
	ERROR ESP Edit : laat de initialisatie lopen en start de klok alvast op, dan d.m.v.millis zoals in de loop de boel synchroniseren als er connectie is.Tevens inbouwen dan tijden < build tijd niet geldig zijn
	ESP niet verbonden / geen internet, na x keer niet meer proberen te connecten / timeout voor nieuwe poging.RTC tijd NIET aanpassen.
   #endif
   //initialize ESP
   if(initESP()) {
		//getEthernetTime();
		//getEthernetDate();
		//time_t newTime = stringToTime(getEthernetDate(), getEthernetTime());
		//setNewTimeRTC(newTime);
    
		if(DebugMode >= 2)
		{
			Serial.println("ESP connected");
		}
	} else {
	  #ifdef DebugMode
		if (DebugMode >= 2)
		{
			Serial.println("ESP connection failed");
		}
		#endif
	}

	ClockState = MODE_TIME;
	ESP_State = ESP_FREE;
}

#pragma region Functions

void updateTimeVar() {
	String newTime = getCurrentTime();
#ifdef DebugMode
	if (DebugMode >= 1)
	{
		Serial.println("current time: " + newTime);
	}
#endif
	char tmpTime[6];
	newTime.toCharArray(tmpTime, 7);

	timeVar[0] = (int)tmpTime[5] - 48;
	timeVar[1] = (int)tmpTime[4] - 48;
	timeVar[2] = (int)tmpTime[3] - 48;
	timeVar[3] = (int)tmpTime[2] - 48;
	timeVar[4] = (int)tmpTime[1] - 48;
	timeVar[5] = (int)tmpTime[0] - 48;
}

void updateDateVar() {
	String newDate = getCurrentDate();
#ifdef DebugMode
	if (DebugMode >= 1)
	{
		Serial.println("current Date: " + newDate);
	}
#endif

	char tmpDate[6];
	newDate.toCharArray(tmpDate, 7);

	dateVar[0] = (int)tmpDate[5] - 48;
	dateVar[1] = (int)tmpDate[4] - 48;
	dateVar[2] = (int)tmpDate[3] - 48;
	dateVar[3] = (int)tmpDate[2] - 48;
	dateVar[4] = (int)tmpDate[1] - 48;
	dateVar[5] = (int)tmpDate[0] - 48;
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
	tmElements_t tmSet;
	tmSet.Year = YYYY - 1970;
	tmSet.Month = MM;
	tmSet.Day = DD;
	tmSet.Hour = hh;
	tmSet.Minute = mm;
	tmSet.Second = ss;
	return makeTime(tmSet);
}

long stringToTime(String curDate, String curTime)
{
	signed char curDay = curDate.substring(0, 2).toInt();
	signed char curMonth = curDate.substring(3, 5).toInt();
	int curYear = (curDate.substring(6, 8).toInt() + 2000);

	signed char curHour = curTime.substring(0, 2).toInt();
	signed char curMin = curTime.substring(3, 5).toInt();
	signed char curSec = curTime.substring(6, 8).toInt();
  
	time_t s_tm = tmConvert_t(curYear, curMonth, curDay, curHour, curMin, curSec);
	return s_tm;
}

void colorTest() {

}
				  
#pragma endregion

#pragma region RunModes
void RunDebugMode() {
	digitalWrite(HVON, HIGH);
	for (int x = 0; x < NumberOfNixies; x++)
	{
		NixiesOff();
		SetNixieTube(x);
		for (int i = 0; i < 10; i++)
		{
			SetNixieDriverVal(i);
			delay(250);
		}
		digitalWrite(LED_RTC, !digitalRead(LED_RTC));
		SwitchDot(DOT_SWITCH);
		//analogWrite(Buzzer, 127);

		colorTest();
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
		ESP_State = ESP_FREE; //temp dummy to get through the code
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
	  if(DebugMode >= 1)
	{
		Serial.println("New ethernet time: " + currentTimeESP);
		Serial.println("New ethernet date: " + currentDateESP);
		if (currentDateESP != "" && currentTimeESP != "") {
			Serial.println("Setting time!");
			time_t newTime = stringToTime(currentDateESP, currentTimeESP);
			setNewTimeRTC(newTime + 35); //compensation for reading serial
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
#pragma endregion


// the loop function runs over and over again forever
#ifdef TODO_LIST
ERROR : implementeer afvangen beschikbare data ESP(aanvraag data beschikbaar ??)
#endif
void loop() {
  
	switch (ClockState) {
	case MODE_DEBUG:
		RunDebugMode();
		ClockState = MODE_TIME;
		break;

	case MODE_TIME:
		RunTimeMode();
		break;

	case MODE_DATE:
		RunDateMode();
		break;

	case MODE_PCP:
		RunPreventionCathodePoisoning(NewClockState);
		break;

	case MODE_UPDATE_TIME:
	  #ifdef DebugMode
		Serial.println("running time update");
	  #endif
		RunUpdateMode();
		#ifdef DebugMode
		Serial.println("Entering clock mode");
	  #endif
		ClockState = MODE_TIME;
		break;
		
	case MODE_UPDATE_ESPDATA:
		RunUpdateESPMode();
		ClockState = CurrentClockState;
		break;

	case MODE_ERROR:
		RunErrorMode();
		break;

	default:
		break;
	}
  
	curMillis = millis();
	if ((curMillis - lastMillisSwitchMode) > SwitchDateTimeInterval || (curMillis - lastMillisSwitchMode) < 0)
	{
		lastMillisSwitchMode = curMillis;
		switch (ClockState) {
		case MODE_TIME:
		  #ifdef DebugMode
			Serial.println("PCP to Date");
		  #endif
			NewClockState = MODE_DATE;
			break;

		case MODE_DATE:
		  #ifdef DebugMode
			Serial.println("PCP to Time");
		  #endif
			NewClockState = MODE_TIME;
			break;

		default:
			Serial.println("Error while switching, current state: " + String(ClockState));
			break;
		}
		ClockState = MODE_PCP;
	}
	if (((curMillis - lastMillisUpdatedESP) > ESPUpdateInterval || (curMillis - lastMillisUpdatedESP) < 0) && !MaxTriesHit)
	{
		lastMillisUpdatedESP = curMillis;
		if (!ConnectedESP)
		{
			CurrentProcessingCommand = IS_ESP_CONNECTED;
		}
		else if (!DateUpdated)
		{
			CurrentProcessingCommand = GET_DATE;
		}
		else if (!TimeUpdated) 
		{
			CurrentProcessingCommand = GET_TIME;
		}
		else
		{
			CurrentProcessingCommand = IS_DATA_UPDATE_AVAIABLE;
		}
		#ifdef DebugMode
		Serial.println("Checking if the ESP has an update available");
		#endif
		CurrentClockState = ClockState;
		ClockState = MODE_UPDATE_ESPDATA;
	}
}
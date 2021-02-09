#include "ESPHandler.h"
#include "NixieClockCore.h"
#include "NixieLighting.h"
#include "Pinout.h"
#include "RunModes.h"
#include "Settings.h"
#include "TimeClient.h"

//--------------Main Program Setup()--------------//
// This is the setup of the NixieClock Firmware
// Called upon starting the MCU
void setup() {
	//Free up PB3 & PB4
	afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  
	//Start Serial communication
	#ifdef DebugMode
	Serial.begin(DebugBAUDRate);
	#endif
	
	#pragma region InitClasses
	if (initRunmodes()) 
	{
		#ifdef DebugMode
		Serial.println("Runmodes initialized");
		#endif
	} 
	else 
	{
		#ifdef DebugMode
		Serial.println("Failed to initialize Runmodes");
		#endif
	}
	
	//set pinmodes
	if(initPins()) {
		#ifdef DebugMode
		Serial.println("Pins initialized");
		#endif
	} else {
		#ifdef DebugMode
		Serial.println("Failed to initialize pins");
		#endif
	}

	//initialize RTC
	if(initTimeClient()) {
		#ifdef DebugMode
		Serial.println("RTC initialized");
		#endif
	} else {
		#ifdef DebugMode
		Serial.println("Failed to initialize RTC");
		#endif
	}

	//initialize WS2811B
	if(initLEDS()) {
		setNeoColor(LED_COLOR_R, LED_COLOR_G, LED_COLOR_B);
		setNeoBrightness(BRIGHTNESS);
		#ifdef DebugMode
		Serial.println("FastLED initialized");
		#endif
	} else {
		#ifdef DebugMode
		Serial.println("Failed to initialize FastLED");
		#endif
	}

	//initialize ESP
   if(initESP()) {
		#ifdef DebugMode
		Serial.println("ESP connected");
		#endif
	} else {
		#ifdef DebugMode
		Serial.println("ESP connection failed");
		#endif
	}
	#pragma endregion
	
	ClockState = MODE_TIME;
	ESP_State = ESP_FREE;
}

//--------------Main Program Loop()--------------//
// This contains the running software of the program
// Once called this will loop forever
void loop() {
  
	switch (ClockState)
	{
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
			#ifdef DebugMode
			Serial.println("Error while switching, current state: " + String(ClockState));
			#endif
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
#include "NixieClockCore.h"

//--------------Main Program Setup()--------------//
// This is the setup of the NixieClock Firmware
// Called upon starting the MCU
void setup() {
	//Free up PB3 & PB4
	afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  
	//Start Serial communication
#ifdef DebugMode
	if(DebugMode > 0)
	{
		Serial.begin(DebugBAUDRate);
	}
#endif
	

	if (initRunmodes()) 
	{
		if (DebugMode >= 2)
		{
			Serial.println("Runmodes initialized");
		}
	} else {
		if (DebugMode >= 2)
		{
			Serial.println("Failed to initialize Runmodes");
		}
	}
	
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

	//initialize RTC
	if(initTimeClient()) {
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

	//initialize WS2811B
	if(initLEDS()) {
		setNeoColor(LED_COLOR_R, LED_COLOR_G, LED_COLOR_B);
		setNeoBrightness(BRIGHTNESS);
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

	//initialize ESP
   if(initESP()) {
		if (DebugMode >= 2)
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

//--------------Main Program Loop()--------------//
// This contains the running software of the program
// Once called this will loop forever
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
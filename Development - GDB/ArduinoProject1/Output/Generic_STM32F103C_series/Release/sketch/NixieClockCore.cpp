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
  
	#pragma region Initialisation
	//Start Serial communication
	#ifdef DebugMode
	Serial.begin(DebugBAUDRate);
	#endif
	if (InitializeSettings())
	{
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Settings initialized");
		#endif
	} 
	else 
	{
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Failed to initialize Settings");
		#endif
	}
	
	if (initRunmodes()) 
	{
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Runmodes initialized");
		#endif
	} 
	else 
	{
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Failed to initialize Runmodes");
		#endif
	}
	
	//set pinmodes
	if(initPins()) {
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Pins initialized");
		#endif
	} else {
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Failed to initialize pins");
		#endif
	}

	//initialize RTC
	if(initTimeClient()) {
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("TimeClient initialized");
		#endif
	} else {
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Failed to initialize TimeClient");
		#endif
	}

	//initialize WS2811B
	if(initLEDS()) {	// re-write needed to go from dumb color writing to support animations etc, functionality is in Runmodes.cpp under RunLightingUpdate()
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("FastLED initialized");
    RunLightingUpdate();
		#endif
	} else {
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Failed to initialize FastLED");
		#endif
	}

	//initialize ESP
   if(initESP()) {
		#ifdef DebugMode
	   if (DebugMode > 0)
		   Serial.println("ESP connected");
		#endif
	} else {
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("ESP connection failed");
		#endif
	}
#pragma endregion
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
		break;

	case MODE_ERROR:
		RunErrorMode();
		break;

	default:
		break;
	}
  
	RunModeUpdate();
}

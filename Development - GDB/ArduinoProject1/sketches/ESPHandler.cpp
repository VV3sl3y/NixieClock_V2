#include "ESPHandler.h"
#include "Settings.h"
#include "Pinout.h"

//#include "SoftSerialSTM32.h"

bool ConnectedESP;
bool MaxTriesHit;
String CurrentProcessingCommand;
int NumberOfConnectionTries;

//SoftSerialSTM32 SWcSerialESP(RX_ESP, TX_ESP); //mcuTX, mcuRX

bool initESP()
{
	ConnectedESP = false;
	MaxTriesHit = false;
	CurrentProcessingCommand = "";
	NumberOfConnectionTries = 0;
	
	Serial2.setTimeout(ESP_ReadTimeOut);
	Serial2.begin(ESP_BAUDRate);

	//activate ESP
	digitalWrite(Flash_ESP, HIGH);
	delay(100); // mandatory delay to make sure ESP Does not start in flash mode
	digitalWrite(RST_ESP, HIGH);
	
	return true;
}

bool sendDataESP(String command)
{
	#ifdef DebugMode
	if(DebugMode > 0)
		Serial.println("now executing sendDataESP(" + command + ")");
	#endif
	Serial2.print(command + ';');

	return true;
}

String receiveDataESP()
{
	#ifdef DebugMode
	if (DebugMode > 0)
		Serial.println("now executing receiveDataESP() ");
	#endif
	String tmpMessage = "";
	
	if (Serial2.available() > 0)
	{
		tmpMessage = Serial2.readStringUntil(';');
		#ifdef DebugMode
		if (DebugMode > 0)
			Serial.println("Following message received from ESP: " + tmpMessage);
		#endif
	}
	else
	{
		return NO_DATA_RETURNED;
	}
	return tmpMessage;
}

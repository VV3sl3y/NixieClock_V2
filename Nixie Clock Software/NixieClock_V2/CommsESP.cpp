#include "CommsESP.h"
#include "Settings.h"
#include "Pinout.h"
#include "SoftSerialSTM32.h"

SoftSerialSTM32 SWSerialESP(RX_ESP, TX_ESP); //mcuTX, mcuRX

bool initESP()
{
	SWSerialESP.setTimeout(ESP_ReadTimeOut);
	SWSerialESP.begin(57600);

	SWSerialESP.listen();
  
	//activate ESP
	digitalWrite(Flash_ESP, HIGH);
	delay(100);
	digitalWrite(RST_ESP, HIGH);
	delay(100);

	return true;
}

bool sendDataESP(String command)
{
#ifdef DebugMode
	if (DebugMode >= 3) {
		Serial.println("now executing sendDataESP() ");
	}
#endif
	SWSerialESP.listen();
	delayMicroseconds(50);
	SWSerialESP.flush();
	delayMicroseconds(50);
	SWSerialESP.print(command + ';');
	delayMicroseconds(50);

	return true;
}

String receiveDataESP()
{
  #ifdef DebugMode
	if (DebugMode >= 3) {
		Serial.println("now executing receiveDataESP() ");
	}
	#endif
	String tmpMessage = "";
	
	//SWSerialESP.listen();
	//delayMicroseconds(200);
	if (SWSerialESP.available() > 0)
	{
		tmpMessage = SWSerialESP.readStringUntil(';');
		#ifdef DebugMode
		if (DebugMode >= 2) {
			Serial.println("Following message received from ESP: " + tmpMessage);
		}
		#endif
	}
	else
	{
		return NO_DATA_RETURNED;
	}
	return tmpMessage;
}

#include <Arduino.h>

#include "Settings.h"

#include <EEPROM.h>

int BRIGHTNESS;
int LED_COLOR_R;
int LED_COLOR_G;
int LED_COLOR_B;
int LED_ANIMATION;

int DisplayTimeUpdateInterval;
int DisplayDateUpdateInterval;
int AmountOfTimeToDisplayTime;
int AmountOfTimeToDisplayDate;
int ESPUpdateInterval;
int DateTimeUpdateInterval;

int NIXIE_OFF_TIME;
int NIXIE_ON_TIME;

int AnimationPCP;
int cyclePCP;
int PCP_INTERVAL;


//ESP Settings
int ESP_BAUDRate;
int ESP_ReadTimeOut;
int Max_ESPConnection_Retries;
int	Max_ESPCommand_Retries;

bool InitializeSettings()
{
	if (1) //ToDo: Check for EEPROM if settings are there, load defaults or EEPROM
	{
		//NixieLighting settings
		BRIGHTNESS		=	32;
		LED_COLOR_R		=	0;
		LED_COLOR_G		=	230;
		LED_COLOR_B		=	255;
		LED_ANIMATION	=	1;
		
		//Times are in milliseconds
		DisplayTimeUpdateInterval	=	100;		//refresh time to request current time
		DisplayDateUpdateInterval	=	900;		//refresh time to request current date
		AmountOfTimeToDisplayTime	=	300000;		//interval between switch time -> date
		AmountOfTimeToDisplayDate	=	30000;		//interval between switch date -> time
		ESPUpdateInterval			=	5000;		//interval between checking if ESP has updates available
		DateTimeUpdateInterval		=	3600000;	//interval to reset the TimeUpdated & DateUpdated
		
		//Tweakable settings for on/off time for each nixie for refresh vs brightness & eliminate ghosting
		NIXIE_OFF_TIME	=	100;
		NIXIE_ON_TIME	=	900;

		/**********************************************************************************
			AnimationPCP: animation between time <-> date switching to save the cathode
			note: numbers from LSB will stop earlier than the MSB (MSB is more static)
			mode 1: Counting up (using a counter to fill the number values ascending)
			mode 2: Counting down (using a counter to fill the number values descending)
			mode 3: Randomized (using rng to fill the number values)
		**********************************************************************************/
		AnimationPCP	=	1;	//selected animation
		cyclePCP		=	20; //number of cycles to do
		PCP_INTERVAL	=	50; //interval between switching numbers
		
		//ESP Settings
		ESP_BAUDRate = 115200; //max proberen 2000000
		ESP_ReadTimeOut = 5;
		Max_ESPConnection_Retries = 5;
		Max_ESPCommand_Retries = 3;
		
	
		HEADER_BOARD UsedHeaderBoard = IN12;
	}
	else
	{
		//EEPROM
	}
}


#pragma message "This is just EEPROM Example code, this has to be properly rewritten to suit the NixieClock Code"
/*
int ledPin =  13;     // LED connected to digital pin 13
const char HELP_MSG[] = "Press :\r\n" \
			" 0 display configuration\r\n" \
			" 1 set configuration to 0x801F000 / 0x801F800 / 0x400 (RB MCU)\r\n" \
			" 2 set configuration to 0x801F000 / 0x801F800 / 0x800 (ZE/RE MCU)\r\n" \
			" 3 write/read variable\r\n" \
			" 4 increment address\r\n" \
			" 5 display pages top/bottom\r\n" \
			" 6 initialize EEPROM\r\n" \
			" 7 format EEPROM\r\n";
uint16 DataWrite = 0;
uint16 AddressWrite = 0x10;

void exampleLoop()
{
	uint16 Status;
	uint16 Data;

	while (Serial.available())
	{
		char cmd = (char)Serial.read();
		Serial.println(cmd);
		if (cmd == '0')
		{
			DisplayConfig();
		}
		else if (cmd == '1')
		{
			EEPROM.PageBase0 = 0x801F000;
			EEPROM.PageBase1 = 0x801F800;
			EEPROM.PageSize  = 0x400;
			DisplayConfig();
		}
		else if (cmd == '2')
		{
			EEPROM.PageBase0 = 0x801F000;
			EEPROM.PageBase1 = 0x801F800;
			EEPROM.PageSize  = 0x800;
			DisplayConfig();
		}
		else if (cmd == '3')
		{
			Status = EEPROM.write(AddressWrite, DataWrite);
			Serial.print("EEPROM.write(0x");
			Serial.print(AddressWrite, HEX);
			Serial.print(", 0x");
			Serial.print(DataWrite, HEX);
			Serial.print(") : Status : ");
			Serial.println(Status, HEX);

			Status = EEPROM.read(AddressWrite, &Data);
			Serial.print("EEPROM.read(0x");
			Serial.print(AddressWrite, HEX);
			Serial.print(", &..) = 0x");
			Serial.print(Data, HEX);
			Serial.print(" : Status : ");
			Serial.println(Status, HEX);

			++DataWrite;
		}
		else if (cmd == '4')
		{
			++AddressWrite;
		}
		else if (cmd == '5')
		{
			DisplayPages(0x20);
			DisplayPagesEnd(0x20);
		}
		else if (cmd == '6')
		{
			Status = EEPROM.init();
			Serial.print("EEPROM.init() : ");
			Serial.println(Status, HEX);
			Serial.println();
		}
		else if (cmd == '7')
		{
			Status = EEPROM.format();
			Serial.print("EEPROM.format() : ");
			Serial.println(Status, HEX);
		}
		else
			Serial.print(HELP_MSG);
	}
}

void DisplayConfig(void)
{
	Serial.print("EEPROM.PageBase0 : 0x");
	Serial.println(EEPROM.PageBase0, HEX);
	Serial.print("EEPROM.PageBase1 : 0x");
	Serial.println(EEPROM.PageBase1, HEX);
	Serial.print("EEPROM.PageSize  : 0x");
	Serial.print(EEPROM.PageSize, HEX);
	Serial.print(" (");
	Serial.print(EEPROM.PageSize, DEC);
	Serial.println(")");
}

void DisplayHex(uint16 value)
{
	if (value <= 0xF)
		Serial.print("000");
	else if (value <= 0xFF)
		Serial.print("00");
	else if (value <= 0xFFF)
		Serial.print("0");
	Serial.print(value, HEX);
}

void DisplayPages(uint32 endIndex)
{
	Serial.println("Page 0     Top         Page 1");

	for (uint32 idx = 0; idx < endIndex; idx += 4)
	{
		Serial.print(EEPROM.PageBase0 + idx, HEX);
		Serial.print(" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx));
		Serial.print(" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx + 2));
		Serial.print("    ");
		Serial.print(EEPROM.PageBase1 + idx, HEX);
		Serial.print(" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx));
		Serial.print(" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx + 2));
		Serial.println();
	}
}

void DisplayPagesEnd(uint32 endIndex)
{
	Serial.println("Page 0     Bottom      Page 1");

	for (uint32 idx = EEPROM.PageSize - endIndex; idx < EEPROM.PageSize; idx += 4)
	{
		Serial.print(EEPROM.PageBase0 + idx, HEX);
		Serial.print(" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx));
		Serial.print(" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx + 2));
		Serial.print("    ");
		Serial.print(EEPROM.PageBase1 + idx, HEX);
		Serial.print(" : ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx));
		Serial.print(" ");
		DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx + 2));
		Serial.println();
	}
}
*/

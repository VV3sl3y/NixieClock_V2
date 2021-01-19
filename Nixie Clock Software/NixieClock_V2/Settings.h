#pragma once

#include <EEPROM.h>

/*
   Debugmode: If needed, this makes sure feedback is received via Serial
   mode 1: Minimalistic: (Check functionallity) Just the basic returns like time, date connection
   mode 2: Intermediate: (Check for errors) Debugging purposes when the clock does not work
   mode 3: Full: (Dive deep) returns all possible states and information during the programm in case mode 2 seems fine
*/
#define DebugMode
#define DebugBAUDRate 115200

#define NumberOfNixies 6

//NixieLighting settings
#define LED_PIN     LED_Data
#define NUM_LEDS    NumberOfNixies
#define LED_TYPE    WS2812B
#define LED_ORDER   GRB
#define BRIGHTNESS		32
#define LED_COLOR_R		0
#define LED_COLOR_G		230
#define LED_COLOR_B		255
#define LED_ANIMATION	 

//Times are in milliseconds
#define DisplayTimeUpdateInterval 100 //refresh time to request current time
#define DisplayDateUpdateInterval 900 //refresh time to request current date
#define SwitchDateTimeInterval 300000 //interval between switch time <-> date
#define ESPUpdateInterval 5000 //interval between checking if ESP has updates available

//Tweakable settings for on/off time for each nixie for refresh vs brightness & eliminate ghosting
#define NIXIE_OFF_TIME 100
#define NIXIE_ON_TIME 900

/*
   AnimationPCP: animation between time <-> date switching to save the cathode
   note: numbers from LSB will stop earlier than the MSB (MSB is more static)
   mode 1: Counting up (using a counter to fill the number values ascending)
   mode 2: Counting down (using a counter to fill the number values descending)
   mode 3: Randomized (using rng to fill the number values)
*/
#define AnimationPCP  1 //selected animation
#define cyclePCP 20     //number of cycles to do
#define PCP_INTERVAL 50 //interval between switching numbers

#define ESP_BAUDRate 57600
#define ESP_ReadTimeOut 5
#define Max_ESPConnection_Retries 5
#define Max_ESPCommand_Retries 3
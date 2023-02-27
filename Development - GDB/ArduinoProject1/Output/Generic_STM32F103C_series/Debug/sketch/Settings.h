#pragma once

/*
   Debugmode: If needed, this makes sure feedback is received via Serial
   mode 1: Minimalistic: (Check functionallity) Just the basic returns like time, date connection
   mode 2: Intermediate: (Check for errors) Debugging purposes when the clock does not work
   mode 3: Full: (Dive deep) returns all possible states and information during the programm in case mode 2 seems fine
*/
#define DebugMode 2
#define DebugBAUDRate 115200

#define NumberOfNixies 6

//NixieLighting settings [fixed settings]
#define LED_PIN     LED_Data
#define LED_TYPE    WS2812B
#define LED_ORDER   GRB
#define NUM_LEDS	NumberOfNixies
//NixieLighting settings [Editable settings]
extern int BRIGHTNESS;
extern int LED_COLOR_R;
extern int LED_COLOR_G;
extern int LED_COLOR_B;
extern int LED_ANIMATION;

//Times are in milliseconds
extern int DisplayTimeUpdateInterval; //refresh time to request current time
extern int DisplayDateUpdateInterval; //refresh time to request current date
extern int SwitchDateTimeInterval; //interval between switch time <-> date
extern int ESPUpdateInterval; //interval between checking if ESP has updates available

//Tweakable settings for on/off time for each nixie for refresh vs brightness & eliminate ghosting
extern int NIXIE_OFF_TIME;
extern int NIXIE_ON_TIME;

/*
   AnimationPCP: animation between time <-> date switching to save the cathode
   note: numbers from LSB will stop earlier than the MSB (MSB is more static)
   mode 1: Counting up (using a counter to fill the number values ascending)
   mode 2: Counting down (using a counter to fill the number values descending)
   mode 3: Randomized (using rng to fill the number values)
*/
extern int AnimationPCP;	//selected animation
extern int cyclePCP;		//number of cycles to do
extern int PCP_INTERVAL;	//interval between switching numbers

extern int ESP_BAUDRate;
extern int ESP_ReadTimeOut;
extern int Max_ESPConnection_Retries;
extern int Max_ESPCommand_Retries;

enum HEADER_BOARD
{
	IN12 = 0,
	IN14
};

extern HEADER_BOARD UsedHeaderBoard;

bool InitializeSettings();

/*void DisplayConfig(void);

void DisplayHex(uint16 value);

void DisplayPages(uint32 endIndex);

void DisplayPagesEnd(uint32 endIndex);
*/
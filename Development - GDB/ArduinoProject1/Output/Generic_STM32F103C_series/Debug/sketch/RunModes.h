#pragma once

long lastMillisCheckedRTC;
long lastMillisPCP;
long lastMillisSwitchMode;
long lastMillisConnectionESP;
long lastMillisUpdatedESP;
long curMillis;

int cycleCurrent;
int FadeInNewMode;

enum CLOCK_MODE
{
	MODE_DEBUG,
	MODE_TIME,
	MODE_DATE,
	MODE_PCP,
	MODE_UPDATE_TIME,
	MODE_UPDATE_ESPDATA,
	MODE_ERROR
};

CLOCK_MODE ClockState;
CLOCK_MODE NewClockState;
CLOCK_MODE CurrentClockState;

enum ESP_Mode
{
	ESP_FREE,
	ESP_AWAITING_ANSWER,
	ESP_COMMAND_REDO
};

ESP_Mode ESP_State;

bool initRunmodes();
void RunDebugMode();
void RunTimeMode();
void RunDateMode();
void RunPreventionCathodePoisoning(CLOCK_MODE);
void RunUpdateESPMode();
void RunModeUpdate(long);
void RunErrorMode();
#pragma once

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

extern CLOCK_MODE ClockState;
extern CLOCK_MODE NewClockState;
extern CLOCK_MODE CurrentClockState;

enum ESP_Mode
{
	ESP_FREE,
	ESP_AWAITING_ANSWER,
	ESP_COMMAND_REDO
};

extern ESP_Mode ESP_State;

bool initRunmodes();
void RunDebugMode();
void RunTimeMode();
void RunDateMode();
void RunPreventionCathodePoisoning(CLOCK_MODE);
void RunUpdateESPMode();
void RunModeUpdate();
void RunErrorMode();
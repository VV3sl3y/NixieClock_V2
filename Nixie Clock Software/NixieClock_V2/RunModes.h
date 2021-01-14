#pragma once

#include <Arduino.h>

#include "CommsESP.h"
#include "DriveNixies.h"
#include "NixieLighting.h"
#include "Pinout.h"
#include "Settings.h"
#include "TimeClient.h"

#include <Time.h>
#include <TimeLib.h>

enum CLOCK_MODE
{
	MODE_DEBUG,
	MODE_TIME,
	MODE_DATE,
	MODE_PCP,
	MODE_UPDATE_TIME,
	MODE_UPDATE_ESPDATA,
	MODE_ERROR
} ClockState;

enum ESP_Mode
{
	ESP_FREE,
	ESP_AWAITING_ANSWER,
	ESP_COMMAND_REDO
} ESP_State;

CLOCK_MODE NewClockState;
CLOCK_MODE CurrentClockState;

bool initRunmodes();
void RunDebugMode();
void RunTimeMode();
void RunDateMode();
void RunPreventionCathodePoisoning(CLOCK_MODE State);
void RunUpdateESPMode();
void RunUpdateMode();
void RunErrorMode();
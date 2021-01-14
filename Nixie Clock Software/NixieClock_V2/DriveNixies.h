#pragma once

#include <Arduino.h>

#include "Pinout.h"

//nixie dot modes
enum dotModes {
	DOT_SWITCH = 0, // switching the dots on/off at 1Hz
	DOT_ON,			// dots constantly on
	DOT_OFF			// dots constantly off
};

enum TubeSelection
{
	SECONDS_LSB = 0,
	SECONDS_MSB,
	MINUTES_LSB,
	MINUTES_MSB,
	HOURS_LSB,
	HOURS_MSB
};

void SetNixieDriverVal(int);
void SetNixieTube(int);
void testNixieTube(int);
void NixiesOff();
void SwitchDot(int);
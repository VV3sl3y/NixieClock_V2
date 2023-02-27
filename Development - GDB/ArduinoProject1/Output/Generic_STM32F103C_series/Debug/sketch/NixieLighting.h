#pragma once

enum NeoAnimations
{
	Off = 0,			//All LEDs off
	Static,				//All LEDs one single color
	Breathing,			//All LEDs in breathing motion ranging between 0 and defined max brightness
	Rainbow,			//Changes the color graduately shifting through the LEDs
	RainbowShift,		//Changes the color graduately over all the LEDs the same
	KnightRider,		//The famous red bar Knight Rider animation (also in other colors)
	WaveRB,				//Turns the LEDs on/off in sequence starting from one side turning on, followed by turning off from that same direction shifting color each wave
	WaveStatic,			//Turns the LEDs on/off in sequence starting from one side turning on, followed by turning off from that same direction
	Strobe,				//Continuously pulse the LEDs
	Pulse				//Short light pulse of the LEDs
};

extern NeoAnimations LightingAnimation;

bool initLEDS();
void setNeoColor(uint32_t r, uint32_t g, uint32_t b);
void setNeoBrightness(uint32);
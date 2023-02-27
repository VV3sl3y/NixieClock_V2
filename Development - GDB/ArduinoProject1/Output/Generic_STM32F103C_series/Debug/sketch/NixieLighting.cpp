#include <Arduino.h>

#include "NixieLighting.h"
#include "Settings.h"
#include "Pinout.h"

//#include "./FastLED/FastLED.h"
#include <FastLED.h>

CRGB leds[NUM_LEDS];

bool initLEDS()
{
	FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, NUM_LEDS);

	return true;
}

void setNeoColor(uint32_t r, uint32_t g, uint32_t b)
{
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB(r, g, b);
	}
	FastLED.show();
}

void setNeoBrightness(uint32 brightness)
{
	FastLED.setBrightness(brightness);
}

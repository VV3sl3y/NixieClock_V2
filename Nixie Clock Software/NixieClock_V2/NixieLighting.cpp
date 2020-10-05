#include "NixieLighting.h"
#include "Pinout.h"

CRGB leds[NUM_LEDS];

bool initLEDS()
{
	FastLED.addLeds<LED_TYPE, LED_PIN, LED_ORDER>(leds, NUM_LEDS);
	FastLED.setBrightness(BRIGHTNESS);

	return true;
}

void setNeoColor(uint32_t r, uint32_t g, uint32_t b)
{
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB(r, g, b);
	}
	FastLED.show();
}

void testFastLED() {
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB::White;
	}
	FastLED.show();
}

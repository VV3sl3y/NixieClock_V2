#include "NixieLighting.h"
#include "Pinout.h"

//CRGB leds[NUM_LEDS];

bool initLEDS()
{
  //FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
  //FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  //FastLED.setBrightness(  BRIGHTNESS );

  return true;
}

void setNeoColor(int r, int g, int b)
{
  //leds[0] = CRGB::White; FastLED.show();
}

void testFastLED() {
  //leds[0] = CRGB::White;

  //FastLED.show();
}

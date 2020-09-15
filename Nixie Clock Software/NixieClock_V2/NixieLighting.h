#include <Arduino.h>
//#include "FastLED.h"

#define LED_PIN     LED_Data
#define NUM_LEDS    6
#define BRIGHTNESS  64
#define LED_TYPE    NEOPIXEL

bool initLEDS();

void setNeoColor(int r, int g, int b);
void testFastLED();

uint8_t red(uint32_t c);
uint8_t green(uint32_t c);
uint8_t blue(uint32_t c);

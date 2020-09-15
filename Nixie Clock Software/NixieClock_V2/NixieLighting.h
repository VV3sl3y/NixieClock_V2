#include <Arduino.h>
#include "Settings.h"
#include "FastLED.h"

#define LED_PIN     LED_Data
#define NUM_LEDS    NumberOfNixies
#define LED_TYPE    WS2812B
#define LED_ORDER   GRB

bool initLEDS();

void setNeoColor(uint32_t r, uint32_t g, uint32_t b);
void testFastLED();

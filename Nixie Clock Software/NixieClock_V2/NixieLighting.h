#pragma once

#include <Arduino.h>

#include "Settings.h"
#include "Pinout.h"

#include <FastLED.h>

#define LED_PIN     LED_Data
#define NUM_LEDS    NumberOfNixies
#define LED_TYPE    WS2812B
#define LED_ORDER   GRB

bool initLEDS();
void setNeoColor(uint32_t r, uint32_t g, uint32_t b);
void setNeoBrightness(uint32);
void testFastLED();
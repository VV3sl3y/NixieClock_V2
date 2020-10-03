#include <Arduino.h>

//nixie dot modes
#define DOT_SWITCH 0
#define DOT_ON 1
#define DOT_OFF 2

void SetNixieDriverVal(int);
void SetNixieTube(int);
void testNixieTube(int);
void NixiesOff();
void SwitchDot(int);
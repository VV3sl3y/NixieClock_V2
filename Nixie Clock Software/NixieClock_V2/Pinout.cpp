#include "Pinout.h"

bool initPins() {
  //initialize pinouts
  pinMode(AlarmOff, INPUT);

  pinMode(LED_Data, OUTPUT);

  pinMode(Flash_ESP, OUTPUT);
  pinMode(RST_ESP, OUTPUT);

  pinMode(Buzzer, OUTPUT);

  pinMode(HVON, OUTPUT);

  pinMode(N_Hour2, OUTPUT);
  pinMode(N_Hour1, OUTPUT);
  pinMode(N_Minute2, OUTPUT);
  pinMode(N_Minute1, OUTPUT);
  pinMode(N_Second2, OUTPUT);
  pinMode(N_Second1, OUTPUT);
  pinMode(N_Dot, OUTPUT);

  pinMode(N_Driver_C, OUTPUT);
  pinMode(N_Driver_B, OUTPUT);
  pinMode(N_Driver_D, OUTPUT);
  pinMode(N_Driver_A, OUTPUT);

  pinMode(LED_RTC, OUTPUT);

  //Set initial state
  digitalWrite(AlarmOff, LOW);

  digitalWrite(LED_Data, LOW);

  //making sure the ESP is reset setting output low for /rst & /flash
  digitalWrite(Flash_ESP, LOW);
  digitalWrite(RST_ESP, LOW);

  digitalWrite(RX_ESP, LOW);
  digitalWrite(TX_ESP, LOW);

  digitalWrite(Buzzer, LOW);

  digitalWrite(HVON, LOW);

  digitalWrite(N_Hour2, LOW);
  digitalWrite(N_Hour1, LOW);
  digitalWrite(N_Minute2, LOW);
  digitalWrite(N_Minute1, LOW);
  digitalWrite(N_Second2, LOW);
  digitalWrite(N_Second1, LOW);
  digitalWrite(N_Dot, LOW);

  digitalWrite(N_Driver_C, LOW);
  digitalWrite(N_Driver_B, LOW);
  digitalWrite(N_Driver_D, LOW);
  digitalWrite(N_Driver_A, LOW);

  digitalWrite(LED_RTC, HIGH);

  return true;
}

#include "DriveNixies.h"
#include "Pinout.h"

//in this function there are all possible pin combos defined, but in reality we'll only use 0 - 9
void SetNixieDriverVal(int i) {
  switch (i)
  {
    case 0:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, LOW);
      break;

    case 1:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, HIGH);
      break;

    case 2:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, HIGH);
      break;

    case 3:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, LOW);
      break;

    case 4:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, LOW);
      break;

    case 5:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, LOW);
      break;
    case 6:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, LOW);
      break;

    case 7:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, LOW);
      break;

    case 8:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, LOW);
      break;

    case 9:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, LOW);
      break;

    case 10:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, HIGH);
      break;

    case 11:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, HIGH);
      break;

    case 12:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, HIGH);
      break;

    case 13:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, HIGH);
      break;
    case 14:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, HIGH);
      break;

    case 15:
      digitalWrite(N_Driver_A, HIGH);
      digitalWrite(N_Driver_B, HIGH);
      digitalWrite(N_Driver_C, HIGH);
      digitalWrite(N_Driver_D, HIGH);
      break;

    default:
      digitalWrite(N_Driver_A, LOW);
      digitalWrite(N_Driver_B, LOW);
      digitalWrite(N_Driver_C, LOW);
      digitalWrite(N_Driver_D, LOW);
      break;
  }
}

void SetNixieTube(int n) {
  switch (n) {
    case 5:
      digitalWrite(N_Hour2, HIGH);
      break;
    case 4:
      digitalWrite(N_Hour1, HIGH);
      break;
    case 3:
      digitalWrite(N_Minute2, HIGH);
      break;
    case 2:
      digitalWrite(N_Minute1, HIGH);
      break;
    case 1:
      digitalWrite(N_Second2, HIGH);
      break;
    case 0:
      digitalWrite(N_Second1, HIGH);
      break;
    default:
      break;
  }
}

void testNixieTube(int n) {
  NixiesOff();
  SetNixieTube(n);
}

void NixiesOff() {
  digitalWrite(N_Hour2, LOW);
  digitalWrite(N_Hour1, LOW);
  digitalWrite(N_Minute2, LOW);
  digitalWrite(N_Minute1, LOW);
  digitalWrite(N_Second2, LOW);
  digitalWrite(N_Second1, LOW);
}

void SwitchDot(int i) {
  switch (i) {
    case DOT_SWITCH:
      digitalWrite(N_Dot, !digitalRead(N_Dot));
      break;
    case DOT_ON:
      digitalWrite(N_Dot, HIGH);
      break;
    case DOT_OFF:
      digitalWrite(N_Dot, LOW);
      break;
  }
}

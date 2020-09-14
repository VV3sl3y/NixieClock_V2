#include "Pinout.h"
#include "DriveNixies.h"
//#include "NixieLighting.h"

void setup() {
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  //initialize pinouts
  pinMode(AlarmOff, OUTPUT);
  
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

  //pinMode(RsvPA15, OUTPUT);
  
  pinMode(N_Driver_C, OUTPUT);
  pinMode(N_Driver_B, OUTPUT);
  pinMode(N_Driver_D, OUTPUT);
  pinMode(N_Driver_A, OUTPUT);
  
  pinMode(RX_ESP,  OUTPUT);
  pinMode(TX_ESP, OUTPUT);
  
  pinMode(LED_RTC, OUTPUT);
  //initNeo();
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(HVON, HIGH);
  for(int x = 0; x < 6; x++)
  {
    NixiesOff();
    SetNixieTube(x);
    for(int i = 0; i < 10; i++)
    {
      SetNixieDriverVal(i);
      delay(250);
    }
    digitalWrite(LED_RTC, !digitalRead(LED_RTC));
    SwitchDot();
    //analogWrite(Buzzer, 127);
  }
}

/*void colorTest(){
  setNeoColor(x, y, z);
      if(x < 255 && updown)
      {
        x++;
        y++;
        z++;
        if(x == 255)
        {
          updown = true;
        }
      }
      else if(x > 0 && !updown)
      {
        x--;
        y--;
        z--;
        if(x == 0)
        {
          updown = false;
        }
      }
}*/

#include "Pinout.h"
#include "Settings.h"
#include "DriveNixies.h"
#include "ClientRTC.h"
#include "NixieLighting.h"
#include "CommsESP.h"

enum CLOCK_MODE
{
  MODE_DEBUG,
  MODE_TIME,
  MODE_DATE,
  MODE_PCP,
  MODE_UPDATE,
  MODE_ERROR
} ClockState;

CLOCK_MODE NewClockState = MODE_TIME;

int lastMillisCheckedRTC = 0;
int lastMillisPCP = 0;
int lastMillisSwitchMode = 0;
int curMillis = 0;

int cycleCurrent = 0;
int FadeInNewMode = 0;

int timeVar[6] = {};
int dateVar[6] = {};
int cathodeVar[6] = {};

void setup() {
  //Free up PB3 & PB4
  //afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  pinF1_DisconnectDebug(PB_3);
  pinF1_DisconnectDebug(PB_4);

  //Start Serial communication
  Serial.begin(115200);

  //set pinmodes
  if (initPins()) {
    if (DebugMode >= 2)
    {
      Serial.println("Pins initialized");
    }
  } else {
    if (DebugMode >= 2)
    {
      Serial.println("Failed to initialize pins");
    }
  }

  //initialize WS2811B
  if (initLEDS()) {
#ifdef DebugMode
    if (DebugMode >= 2)
    {
      Serial.println("WS2811 initialized");
    }
#endif
  } else {
#ifdef DebugMode
    if (DebugMode >= 2)
    {
      Serial.println("Failed to initialize WS2811B");
    }
#endif
  }

  //initialize RTC
  if (initRTC()) {
    if (DebugMode >= 2)
    {
      Serial.println("RTC initialized");
    }
  } else {
    if (DebugMode >= 2)
    {
      Serial.println("Failed to initialize RTC");
    }
  }

  //initialize ESP
  if (initESP()) {
#ifdef DebugMode
    if (DebugMode >= 2)
    {
      Serial.println("ESP connected");
    }
#endif
  } else {
#ifdef DebugMode
    if (DebugMode >= 2)
    {
      Serial.println("ESP connection failed");
    }
#endif
  }

  ClockState = MODE_TIME;
}

// the loop function runs over and over again forever
void loop() {
  switch (ClockState) {
    case MODE_DEBUG:
      RunDebugMode();
      ClockState = MODE_UPDATE;
      break;

    case MODE_TIME:
      RunTimeMode();
      break;

    case MODE_DATE:
      RunDateMode();
      break;

    case MODE_PCP:
      RunPreventionCathodePoisoning(NewClockState);
      break;

    case MODE_UPDATE:
#ifdef DebugMode
      Serial.println("running time update");
#endif
      RunUpdateMode();
#ifdef DebugMode
      Serial.println("Entering clock mode");
#endif
      ClockState = MODE_TIME;
      break;

    case MODE_ERROR:
      RunErrorMode();
      break;

    default:
      break;
  }
  
  curMillis = millis();
  if ((curMillis - lastMillisSwitchMode) > SwitchDateTimeInterval)
  {
    lastMillisSwitchMode = curMillis;
    switch(ClockState){
      case MODE_TIME:
        #ifdef DebugMode
          Serial.println("PCP to Date");
        #endif
        NewClockState = MODE_DATE;
        break;

      case MODE_DATE:
        #ifdef DebugMode
          Serial.println("PCP to Time");
        #endif
        NewClockState = MODE_TIME;
        break;

      default:
        Serial.println("Error while switching, current state: " + String(ClockState));
        break;
    }
    ClockState = MODE_PCP;
  }
}

void RunDebugMode() {
  digitalWrite(HVON, HIGH);
  for (int x = 0; x < NumberOfNixies; x++)
  {
    NixiesOff();
    SetNixieTube(x);
    for (int i = 0; i < 10; i++)
    {
      SetNixieDriverVal(i);
      delay(250);
    }
    digitalWrite(LED_RTC, !digitalRead(LED_RTC));
    SwitchDot(DOT_SWITCH);
    //analogWrite(Buzzer, 127);

    colorTest();
  }
}

void RunTimeMode() {
  testFastLED();

  curMillis = millis();
  if ((curMillis - lastMillisCheckedRTC) > DisplayTimeUpdateInterval)
  {
    updateTimeVar();

    lastMillisCheckedRTC = curMillis;

#ifdef DebugMode
    if (DebugMode >= 2)
    {
      Serial.println("curmillis:" + String(curMillis) + "   oldMillisTimeRTC:" + String(lastMillisCheckedRTC));
    }
#endif

    if ((timeVar[0] % 2) == 1)
    {
      SwitchDot(DOT_ON);
    }
    else
    {
      SwitchDot(DOT_OFF);
    }
  }

  digitalWrite(HVON, HIGH);
  for (int x = 0; x < NumberOfNixies; x++)
  {
    NixiesOff();
    delayMicroseconds(NIXIE_OFF_TIME);
    SetNixieDriverVal(timeVar[x]);
    SetNixieTube(x);

    delayMicroseconds(NIXIE_ON_TIME);
  }
}

void RunDateMode() {
  testFastLED();
  
  curMillis = millis();
  if ((curMillis - lastMillisCheckedRTC) > DisplayDateUpdateInterval)
  {
    updateDateVar();

    lastMillisCheckedRTC = curMillis;
#ifdef DebugMode
    if (DebugMode >= 2)
    {
      Serial.println("curmillis:" + String(curMillis) + "   oldMillisDateRTC:" + String(lastMillisCheckedRTC));
    }
#endif
  }

  digitalWrite(HVON, HIGH);
  for (int x = 0; x < NumberOfNixies; x++)
  {
    NixiesOff();
    delayMicroseconds(NIXIE_OFF_TIME);
    SetNixieDriverVal(dateVar[x]);
    SetNixieTube(x);

    delayMicroseconds(NIXIE_ON_TIME);
  }
  SwitchDot(DOT_ON);
}

void RunPreventionCathodePoisoning(CLOCK_MODE State) {
  curMillis = millis();
  if((curMillis - lastMillisPCP) > PCP_INTERVAL){
    lastMillisPCP = millis();
    if(State == MODE_TIME){
      updateTimeVar();
      for(int i = 0; i < FadeInNewMode; i++){
        cathodeVar[i] = timeVar[i];
      }
    }
    else if(State == MODE_DATE){
      updateDateVar();
      for(int i = 0; i < FadeInNewMode; i++){
        cathodeVar[i] = dateVar[i];
      }
    }
    
    if(AnimationPCP == 1){
      for(int i = FadeInNewMode; i < NumberOfNixies; i++){
        cathodeVar[i] = cathodeVar[i] + 1;
        if(cathodeVar[i] > 9) {
          cathodeVar[i] = 0;
        }
      }
    }
    else if(AnimationPCP == 2){
        for(int i = FadeInNewMode; i < NumberOfNixies; i++){
          cathodeVar[i] = cathodeVar[i] - 1;
          if(cathodeVar[i] < 0) {
            cathodeVar[i] = 9;
          }
        }
    }

    else if(AnimationPCP == 3){
      for(int i = FadeInNewMode; i < NumberOfNixies; i++){
        cathodeVar[i] = random(10);
      }
    }

    else {
        Serial.println("Error while animating, set animation: " + AnimationPCP);
    }

    if(cycleCurrent < cyclePCP){
      cycleCurrent++;
    }
    else if(FadeInNewMode < NumberOfNixies){
      FadeInNewMode++;
      cycleCurrent = 0;
    }
    else
    {
      Serial.println("entering new afterpcp state: " + String(ClockState));
      cycleCurrent = 0;
      FadeInNewMode = 0;
      ClockState = NewClockState;
    }
#ifdef DebugMode
    if (DebugMode >= 2)
    {
      Serial.println("curmillis:" + String(curMillis) + "   oldMillisPCP:" + String(lastMillisPCP));
    }
#endif
  }
  
  digitalWrite(HVON, HIGH);
  for (int x = 0; x < NumberOfNixies; x++)
  {
    NixiesOff();
    delayMicroseconds(NIXIE_OFF_TIME);
    SetNixieDriverVal(cathodeVar[x]);
    SetNixieTube(x);

    delayMicroseconds(NIXIE_ON_TIME);
  }
}

void RunUpdateMode()
{
  String currentTimeESP = getEthernetTime();
  String currentDateESP = getEthernetDate();

#ifdef DebugMode
  if (DebugMode >= 1)
  {
    Serial.println("New ethernet time: " + currentTimeESP);
    Serial.println("New ethernet date: " + currentDateESP);
  }
#endif

  char tmpTime[8];
  currentTimeESP.toCharArray(tmpTime , 8);

  timeVar[0] = (int)tmpTime[7] - 48;
  timeVar[1] = (int)tmpTime[6] - 48;
  timeVar[2] = (int)tmpTime[4] - 48;
  timeVar[3] = (int)tmpTime[3] - 48;
  timeVar[4] = (int)tmpTime[1] - 48;
  timeVar[5] = (int)tmpTime[0] - 48;

  char tmpDate[8];
  currentTimeESP.toCharArray(tmpDate , 8);

  dateVar[0] = (int)tmpDate[7] - 48;
  dateVar[1] = (int)tmpDate[6] - 48;
  dateVar[2] = (int)tmpDate[4] - 48;
  dateVar[3] = (int)tmpDate[3] - 48;
  dateVar[4] = (int)tmpDate[1] - 48;
  dateVar[5] = (int)tmpDate[0] - 48;
}

void RunErrorMode() {

}

void updateTimeVar(){
  String newTime = getCurrentTime();
#ifdef DebugMode
    if (DebugMode >= 1)
    {
      Serial.println("current time: " + newTime);
    }
#endif
    char tmpTime[6];
    newTime.toCharArray(tmpTime , 7);

    timeVar[0] = (int)tmpTime[5] - 48;
    timeVar[1] = (int)tmpTime[4] - 48;
    timeVar[2] = (int)tmpTime[3] - 48;
    timeVar[3] = (int)tmpTime[2] - 48;
    timeVar[4] = (int)tmpTime[1] - 48;
    timeVar[5] = (int)tmpTime[0] - 48;
}

void updateDateVar(){
  String newDate = getCurrentDate();
#ifdef DebugMode
    if (DebugMode >= 1)
    {
      Serial.println("current Date: " + newDate);
    }
#endif

    char tmpDate[6];
    newDate.toCharArray(tmpDate , 7);

    dateVar[0] = (int)tmpDate[5] - 48;
    dateVar[1] = (int)tmpDate[4] - 48;
    dateVar[2] = (int)tmpDate[3] - 48;
    dateVar[3] = (int)tmpDate[2] - 48;
    dateVar[4] = (int)tmpDate[1] - 48;
    dateVar[5] = (int)tmpDate[0] - 48;
}

void colorTest() {

}

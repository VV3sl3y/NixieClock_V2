/*
  This is an example file for using the time function in ESP8266 or ESP32 tu get NTP time
  It offers two functions:

  - getNTPtime(struct tm * info, uint32_t ms) where info is a structure which contains time
  information and ms is the time the service waits till it gets a response from NTP.
  Each time you cann this function it calls NTP over the net.

  If you do not want to call an NTP service every second, you can use
  - getTimeReducedTraffic(int ms) where ms is the the time between two physical NTP server calls. Betwwn these calls,
  the time structure is updated with the (inaccurate) timer. If you call NTP every few minutes you should be ok

  The time structure is called tm and has teh following values:

  Definition of struct tm:
  Member  Type  Meaning Range
  tm_sec  int seconds after the minute  0-61*
  tm_min  int minutes after the hour  0-59
  tm_hour int hours since midnight  0-23
  tm_mday int day of the month  1-31
  tm_mon  int months since January  0-11
  tm_year int years since 1900
  tm_wday int days since Sunday 0-6
  tm_yday int days since January 1  0-365
  tm_isdst  int Daylight Saving Time flag

  because the values are somhow akwardly defined, I introduce a function makeHumanreadable() where all values are adjusted according normal numbering.
  e.g. January is month 1 and not 0 And Sunday or monday is weekday 1 not 0 (according definition of MONDAYFIRST)

  Showtime is an example on how you can use the time in your sketch

  The functions are inspired by work of G6EJD ( https://www.youtube.com/channel/UCgtlqH_lkMdIa4jZLItcsTg )

*/

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <time.h>

const char* ssid = "Not The Droid You're Looking For";
const char* password = "HeavyMetalFan94";

const char* NTP_SERVER = "ch.pool.ntp.org";
const char* TZ_INFO    = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";  // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;


void setup() {
  Serial.begin(115200);
  Serial.println("\n\nNTP Time Test\n");
  WiFi.begin(ssid, password);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (++counter > 100) ESP.restart();
    Serial.print ( "." );
  }
  Serial.println("\n\nWiFi connected\n\n");

  configTime(0, 0, NTP_SERVER);
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", TZ_INFO, 1);

  if (getNTPtime(60)) {  // wait up to 60sec to sync
  } else {
    Serial.println("Time not set");
    ESP.restart();
  }
  showTime(timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
}

/*
Als dit werkt, todo:
- inbouwen in de Nixie_ESP software
- comms testen met STM controller
- als dit niet werkt bet Baud kloten
- in Main software initialisatie aanpassen. Eerst initialiseren en de klok gewoon laten lopen, vervolgens na X millis de klok pas laten kijken of deze connected is en nieuwe tijden etc heeft
*/
void loop() {
  getNTPtime(60);
  showTime(timeinfo);
  showDate(timeinfo);
  showEpoch();
  delay(1000);
}

bool getNTPtime(int sec) {

  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
  }
  return true;
}

void showTime(tm localTime) {
  Serial.print(CheckLeadingZero(localTime.tm_hour));
  Serial.print(':');
  Serial.print(CheckLeadingZero(localTime.tm_min));
  Serial.print(':');
  Serial.println(CheckLeadingZero(localTime.tm_sec));
}

void showDate(tm localTime) {
  Serial.print(CheckLeadingZero(localTime.tm_mday));
  Serial.print('/');
  Serial.print(CheckLeadingZero(localTime.tm_mon + 1));
  Serial.print('/');
  Serial.println(CheckLeadingZero(localTime.tm_year - 100));
}

void showEpoch(){
  Serial.print("now: ");  Serial.println(now);
}

void showDateTime(tm localTime) {
  Serial.print(CheckLeadingZero(localTime.tm_mday));
  Serial.print('/');
  Serial.print(CheckLeadingZero(localTime.tm_mon + 1));
  Serial.print('/');
  Serial.print(CheckLeadingZero(localTime.tm_year - 100));
  Serial.print('-');
  Serial.print(CheckLeadingZero(localTime.tm_hour));
  Serial.print(':');
  Serial.print(CheckLeadingZero(localTime.tm_min));
  Serial.print(':');
  Serial.print(CheckLeadingZero(localTime.tm_sec));
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)   Serial.println(7);
  else Serial.println(localTime.tm_wday);
}

String CheckLeadingZero(int TimeDate){
  if(TimeDate < 10){
    return "0" + String(TimeDate);
  }
  return String(TimeDate);
}

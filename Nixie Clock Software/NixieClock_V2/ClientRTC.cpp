#include <STM32RTC.h>
#include "ClientRTC.h"

/* Get the rtc object */
STM32RTC& rtc = STM32RTC::getInstance();

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 51;
const byte hours = 17;

/* Change these values to set the current initial date */
/* Tuesday 31th December 2019 */
const byte weekDay = 2;
const byte day = 30;
const byte month = 12;
const byte year = 19;

bool initRTC()
{
  // Select RTC clock source: LSI_CLOCK, LSE_CLOCK or HSE_CLOCK.
  // By default the LSI is selected as source.
  //LSI = Low Speed Internal; LSE = Low Speed External; HSE = High Speed External
  rtc.setClockSource(STM32RTC::LSE_CLOCK);

  rtc.begin(); // initialize RTC 24H format

  // Set the time
  //rtc.setHours(hours);
  //rtc.setMinutes(minutes);
  //rtc.setSeconds(seconds);

  // Set the date
  //rtc.setWeekDay(weekDay);
  //rtc.setDay(day);
  //rtc.setMonth(month);
  //rtc.setYear(year);

  //rtc.setEpoch(1577725800);

  return true;
}

String getCurrentTime()
{
  String curTime = "";
  // Get time
  curTime += print2digits(rtc.getHours());
  //curTime += ":";
  curTime += print2digits(rtc.getMinutes());
  //curTime += ":";
  curTime += print2digits(rtc.getSeconds());

  return curTime;
}

String getCurrentDate()
{
  String curDate = "";
  // Get date
  curDate += print2digits(rtc.getDay());
  //curTime += "-";
  curDate += print2digits(rtc.getMonth());
  //curTime += "-";
  curDate += print2digits(rtc.getYear());

  return curDate;
}

String print2digits(int number) {
  String tmpData = String(number);
  if (number < 10) {
    tmpData = "0" + String(number); // print a 0 before if the number is < than 10
    return tmpData;
  }
  return String(number);
}

/*bool UpdateRTC()
  {

  }*/

/*bool SetAlarm()
  {

  }*/

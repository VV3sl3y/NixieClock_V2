#include <RTClock.h>
#include "ClientRTC.h"

RTClock rtclock (RTCSEL_LSE); // initialise
time_t tt, tt1;
tm_t mtt;
uint8_t dateread[11];

//-----------------------------------------------------------------------------
const char * weekdays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char * months[] = {"Dummy", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
//-----------------------------------------------------------------------------
uint8_t str2month(const char * d)
{
	uint8_t i = 13;
	while ((--i) && strcmp(months[i], d) != 0) ;
	return i;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void ParseBuildTimestamp(tm_t & mt)
{
	// Timestamp format: "Dec  8 2017, 22:57:54"
	sprintf(s, "Timestamp: %s, %s\n", __DATE__, __TIME__);
	char * token = strtok(s, delim);  // get first token
	// walk through tokens
	while(token != NULL) {
		uint8_t m = str2month((const char*)token);
		if (m > 0) {
			mt.month = m;
			token = strtok(NULL, delim);  // get next token
			mt.day = atoi(token);
			token = strtok(NULL, delim);  // get next token
			mt.year = atoi(token) - 1970;
			token = strtok(NULL, delim);  // get next token
			mt.hour = atoi(token);
			token = strtok(NULL, delim);  // get next token
			mt.minute = atoi(token);
			token = strtok(NULL, delim);  // get next token
			mt.second = atoi(token);
		}
		token = strtok(NULL, delim);
	}
}

//-----------------------------------------------------------------------------
// This function is called in the attachSecondsInterrupt
//-----------------------------------------------------------------------------
void SecondCount()
{
	tt++;
}

//-----------------------------------------------------------------------------
// This function is called in the attachAlarmInterrupt
//-----------------------------------------------------------------------------
void blink()
{
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
//-----------------------------------------------------------------------------

bool initRTC()
{
	rtclock.breakTime(rtclock.now(), mtt);
	tt = rtclock.makeTime(mtt);  // additional seconds to compensate build and upload delay
	tt1 = tt;
	rtclock.attachAlarmInterrupt(blink); // Call blink
	rtclock.attachSecondsInterrupt(SecondCount); // Call SecondCount
  
	return true;
}

String getCurrentTime()
{
	String curTime = "";
	// Get time
	rtclock.breakTime(rtclock.now(), mtt);
	curTime += print2digits(mtt.hour);
	//curTime += ":";
	curTime += print2digits(mtt.minute);
	//curTime += ":";
	curTime += print2digits(mtt.second);

	return curTime;
}

String getCurrentDate()
{
	String curDate = "";
	// Get date
	rtclock.breakTime(rtclock.now(), mtt);
	curDate += print2digits(mtt.day);
	//curTime += "-";
	curDate += print2digits(mtt.month);
	//curTime += "-";
	curDate += print2digits(mtt.year - 30);  //timenotation +1970 for current year, -2000 for propper show on nixies (last 2 digits)

	return curDate;
}

String print2digits(int number) {
	String tmpData = String(number);
	if (number < 10) {
		tmpData = "0" + String(number);  // print a 0 before if the number is < than 10
		return tmpData;
	}
	return String(number);
}

void setNewTimeRTC(time_t newTime) {
	//if(tt > mt)//check if new set time is larger then ParseBuildTime
	//{
	Serial.print("Trying to set new time: ");
	Serial.println(newTime);
	rtclock.setTime(newTime);
	//}
}

void setAlarmRTC() {
	//rtclock.detachAlarmInterrupt();
	//rtclock.createAlarm(OnOffSerial, (rtclock.getTime() + 20));   // call OnOffSerial stop output date from Serial after 2 mins
}

String getCurrentDateTime() {
	if (tt1 != tt)
	{
		tt1 = tt;
		// get and print actual RTC timestamp
		rtclock.breakTime(rtclock.now(), mtt);
		sprintf(s,
			"RTC timestamp: %s %u %u, %s, %02u:%02u:%02u\n",
			months[mtt.month],
			mtt.day,
			mtt.year+1970,
			weekdays[mtt.weekday],
			mtt.hour,
			mtt.minute,
			mtt.second);
		Serial.print(s);
	}
	return s;
}

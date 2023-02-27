#include "TimeClient.h"

#include <Time.h>
#include <TimeLib.h>
#include <RTClock.h>

int timeVar[6];
int dateVar[6];
int cathodeVar[6];

bool DateUpdated;
bool TimeUpdated;
bool RTCUpdated;
String currentDateESP;
String currentTimeESP;

const char* delim = " :";
char s[128];   // for sprintf

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
void ParseBuildTimestamp(tm_t &mt)
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

bool initTimeClient()
{
	rtclock.breakTime(rtclock.now(), mtt);
	tt = rtclock.makeTime(mtt);  // additional seconds to compensate build and upload delay
	tt1 = tt;
	rtclock.attachAlarmInterrupt(blink); // Call blink
	rtclock.attachSecondsInterrupt(SecondCount); // Call SecondCount
  
	for (int i = 0; i < 6; i++) {
		timeVar[i] = 0;
	}
	for (int i = 0; i < 6; i++) {
		dateVar[i] = 0;
	}
	
	for (int i = 0; i < 6; i++) {
		cathodeVar[i] = 0;
	}

	DateUpdated = false;
	TimeUpdated = false;
	RTCUpdated = false;
	currentDateESP = "";
	currentTimeESP = "";
	
	return true;
}

String getCurrentTime()
{
	String curTime = "";
	rtclock.breakTime(rtclock.now(), mtt);
	curTime += print2digits(mtt.hour);
	curTime += print2digits(mtt.minute);
	curTime += print2digits(mtt.second);

	return curTime;
}

String getCurrentDate()
{
	String curDate = "";
	rtclock.breakTime(rtclock.now(), mtt);
	curDate += print2digits(mtt.day);
	curDate += print2digits(mtt.month);
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
	//if(newTime > mt)//check if new set time is larger then ParseBuildTime
	//{
	#ifdef DebugMode
	Serial.print("Trying to set new time: ");
	Serial.println(newTime);
	#endif
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
		#ifdef DebugMode
		Serial.print(s);
		#endif
	}
	return s;
}

//--------------Main TimeControl--------------//
// This is the main functionality of the program
// Where the time to display is refreshed etc.
#pragma region TimeControl

bool updateTimeVar() {
	String newTime = getCurrentTime();
	#ifdef DebugMode
	Serial.println("current time: " + newTime);
	#endif
	char tmpTime[6];
	newTime.toCharArray(tmpTime, 7);

	timeVar[0] = (int)tmpTime[5] - 48;
	timeVar[1] = (int)tmpTime[4] - 48;
	timeVar[2] = (int)tmpTime[3] - 48;
	timeVar[3] = (int)tmpTime[2] - 48;
	timeVar[4] = (int)tmpTime[1] - 48;
	timeVar[5] = (int)tmpTime[0] - 48;
	
	return true;
}

bool updateDateVar() {
	String newDate = getCurrentDate();
	#ifdef DebugMode
	Serial.println("current Date: " + newDate);
	#endif

	char tmpDate[6];
	newDate.toCharArray(tmpDate, 7);

	dateVar[0] = (int)tmpDate[5] - 48;
	dateVar[1] = (int)tmpDate[4] - 48;
	dateVar[2] = (int)tmpDate[3] - 48;
	dateVar[3] = (int)tmpDate[2] - 48;
	dateVar[4] = (int)tmpDate[1] - 48;
	dateVar[5] = (int)tmpDate[0] - 48;
	
	return true;
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
	tmElements_t tmSet;
	tmSet.Year = YYYY - 1970;
	tmSet.Month = MM;
	tmSet.Day = DD;
	tmSet.Hour = hh;
	tmSet.Minute = mm;
	tmSet.Second = ss;
	return makeTime(tmSet);
}

long stringToTime(String curDate, String curTime)
{
	signed char curDay = curDate.substring(0, 2).toInt();
	signed char curMonth = curDate.substring(3, 5).toInt();
	int curYear = (curDate.substring(6, 8).toInt() + 2000);

	signed char curHour = curTime.substring(0, 2).toInt();
	signed char curMin = curTime.substring(3, 5).toInt();
	signed char curSec = curTime.substring(6, 8).toInt();
  
	time_t s_tm = tmConvert_t(curYear, curMonth, curDay, curHour, curMin, curSec);
	return s_tm;
}
				  
#pragma endregion
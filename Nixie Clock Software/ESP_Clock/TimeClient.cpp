/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
Modified by David Payne for use in the Scrolling Marquee
*/

#include "TimeClient.h"
#include <TimeLib.h>

TimeClient::TimeClient(float utcOffset) {
  myUtcOffset = utcOffset;
}

void TimeClient::updateTime() {
  WiFiClient client;
  
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (++counter > 250) ESP.restart();
  }
  Serial.println("\n\nWiFi connected\n\n");

  // This will send the request to the server
  configTime(0, 0, ntpServer);
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", TZ_INFO, 1);
  
  uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
    } while (((millis() - start) <= (1000 * 60)) && (timeinfo.tm_year < (2016 - 1900)));
  
  Serial.println(getFormattedTime());
  Serial.println(getFormattedDate());
  localEpoc = time(&now);
  localMillisAtUpdate = millis();
}

void TimeClient::setUtcOffset(float utcOffset) {
	myUtcOffset = utcOffset;
}

String TimeClient::getAmPmHours() {
	int hours = timeinfo.tm_hour;
	if (hours >= 13) {
		hours = hours - 12;
	}
	if (hours == 0) {
		hours = 12;
	}
	return String(hours);
}

String TimeClient::getAmPm() {
	int hours = timeinfo.tm_hour;
	String ampmValue = "AM";
	if (hours >= 12) {
		ampmValue = "PM";
	}
	return ampmValue;
}

String TimeClient::getFormattedTime() {
  return CheckLeadingZero(timeinfo.tm_hour) + ":" + CheckLeadingZero(timeinfo.tm_min) + ":" + CheckLeadingZero(timeinfo.tm_sec);
}

String TimeClient::getAmPmFormattedTime() {
	return getAmPmHours() + ":" + CheckLeadingZero(timeinfo.tm_min) + " " + getAmPm();
}

String TimeClient::getFormattedDate(){
  return CheckLeadingZero(timeinfo.tm_mday) + "-" + CheckLeadingZero(timeinfo.tm_mon + 1) + "-" + CheckLeadingZero(timeinfo.tm_year - 100);
}

String TimeClient::CheckLeadingZero(int TimeDate) {
    if (TimeDate < 10) {
        return "0" + String(TimeDate);
    }
    return String(TimeDate);
}

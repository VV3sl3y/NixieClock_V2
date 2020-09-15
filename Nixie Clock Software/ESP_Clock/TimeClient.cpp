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
  
  if (!client.connect(ntpServerName, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.print(String("GET / HTTP/1.1\r\n") +
               String("Host: www.google.com\r\n") +
               String("Connection: close\r\n\r\n"));
  int repeatCounter = 0;
  while(!client.available() && repeatCounter < 10) {
    delay(1000);
    Serial.println(".");
    repeatCounter++;
  }

  String line;

  int size = 0;
  client.setNoDelay(false);
  while(client.connected()) {
    while((size = client.available()) > 0) {
      line = client.readStringUntil('\n');
      line.toUpperCase();
      // example:
      // date: Thu, 19 Nov 2015 20:25:40 GMT
      if (line.startsWith("DATE: ")) {
        Serial.println(line.substring(23, 25) + ":" + line.substring(26, 28) + ":" +line.substring(29, 31));
        int parsedHours = line.substring(23, 25).toInt();
        int parsedMinutes = line.substring(26, 28).toInt();
        int parsedSeconds = line.substring(29, 31).toInt();
        int parsedDays = line.substring(12, 14).toInt();
        int parsedMonths = ConvertToNumericDate(line.substring(14, 17));
        int parsedYears = line.substring(18, 22).toInt();
        Serial.println(String(parsedHours) + ":" + String(parsedMinutes) + ":" + String(parsedSeconds));
        Serial.println(String(parsedDays) + ":" + String(parsedMonths) + ":" + String(parsedYears));
        localEpoc = ((parsedYears - 1970) * 365 * 24 * 60 * 60 + parsedMonths * 30 * 60 * 60 + parsedDays * 24 * 60 * 60 + parsedHours * 60 * 60 + parsedMinutes * 60 + parsedSeconds); //not counting the months due to them not being needed to calculate epoch
        Serial.println(localEpoc);
        localMillisAtUpdate = millis();
        client.stop();
      }
    }
  }
}

void TimeClient::setUtcOffset(float utcOffset) {
	myUtcOffset = utcOffset;
}

String TimeClient::getHours() {
    if (localEpoc == 0) {
      return "--";
    }
    int hours = ((getCurrentEpochWithUtcOffset()  % 86400L) / 3600) % 24;
    if (hours < 10) {
      return "0" + String(hours);
    }
    return String(hours); // print the hour (86400 equals secs per day)

}
String TimeClient::getMinutes() {
    if (localEpoc == 0) {
      return "--";
    }
    int minutes = ((getCurrentEpochWithUtcOffset() % 3600) / 60);
    if (minutes < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      return "0" + String(minutes);
    }
    return String(minutes);
}
String TimeClient::getSeconds() {
    if (localEpoc == 0) {
      return "--";
    }
    int seconds = getCurrentEpochWithUtcOffset() % 60;
    if ( seconds < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      return "0" + String(seconds);
    }
    return String(seconds);
}

String TimeClient::getDay() {
    if (localEpoc == 0) {
      return "--";
    }
    int days = getCurrentEpochWithUtcOffset() % (24 * 60 * 60);
    if ( days < 10 ) {
      // In the first 10 days of each minute, we'll want a leading '0'
      return "0" + String(days);
    }
    return String(days);
}
String TimeClient::getMonth() {
    if (localEpoc == 0) {
      return "--";
    }
    int months = getCurrentEpochWithUtcOffset() % (30 * 24 * 60 * 60);
    if ( months < 10 ) {
      // In the first 10 months of each minute, we'll want a leading '0'
      return "0" + String(months);
    }
    return String(months);
}
String TimeClient::getYear() {
    if (localEpoc == 0) {
      return "--";
    }
    int years = (getCurrentEpochWithUtcOffset() % (365 * 24 * 60 * 60)) + 50;
    if ( years < 10 ) {
      // In the first 10 years of each minute, we'll want a leading '0'
      return "0" + String(years);
    }
    return String(years);
}

int TimeClient::ConvertToNumericDate(String curMonth){
  if(curMonth == "JAN"){ return 1; }
  else if(curMonth == "FEB"){ return 2; }
  else if(curMonth == "MAR"){ return 3; }
  else if(curMonth == "APR"){ return 4; }
  else if(curMonth == "MAY"){ return 5; }
  else if(curMonth == "JUN"){ return 6; }
  else if(curMonth == "JUL"){ return 7; }
  else if(curMonth == "AUG"){ return 8; }
  else if(curMonth == "SEP"){ return 9; }
  else if(curMonth == "OCT"){ return 10; }
  else if(curMonth == "NOV"){ return 11; }
  else if(curMonth == "DEC"){ return 12; }
  else { return 0; }
}

String TimeClient::getAmPmHours() {
	int hours = getHours().toInt();
	if (hours >= 13) {
		hours = hours - 12;
	}
	if (hours == 0) {
		hours = 12;
	}
	return String(hours);
}

String TimeClient::getAmPm() {
	int hours = getHours().toInt();
	String ampmValue = "AM";
	if (hours >= 12) {
		ampmValue = "PM";
	}
	return ampmValue;
}

String TimeClient::getFormattedTime() {
  return getHours() + ":" + getMinutes() + ":" + getSeconds();
}

String TimeClient::getAmPmFormattedTime() {
	return getAmPmHours() + ":" + getMinutes() + " " + getAmPm();
}

String TimeClient::getFormattedDate(){
  return getDay() + "-" + getMonth() + "-" + getYear();
}

long TimeClient::getCurrentEpoch() {
  return localEpoc + ((millis() - localMillisAtUpdate) / 1000);
}

long TimeClient::getCurrentEpochWithUtcOffset() {
  return (long)round(getCurrentEpoch() + 3600 * myUtcOffset + 86400L) % 86400L;
}

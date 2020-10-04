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

See more at http://blog.squix.ch
*/

/*
Modified by David Payne for use in the Scrolling Marquee
*/

#pragma once

#include <ESP8266WiFi.h>

#define NTP_PACKET_SIZE 48

class TimeClient {

  private:
    float myUtcOffset = 0;
    unsigned long localEpoc = 0;
    unsigned long localMillisAtUpdate;

    tm timeinfo;
    time_t now;
    
    const char* ntpServer = "ch.pool.ntp.org";
    const char* TZ_INFO    = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";  // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)
    const int httpPort = 80;    
    
    byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

  public:
    TimeClient(float utcOffset);
    void updateTime();
    
    void setUtcOffset(float utcOffset);

    String getAmPmHours();
    String getAmPm();

    String CheckLeadingZero(int);
    int ConvertToNumericMonth(String);
    
    String getFormattedTime();
    String getAmPmFormattedTime();
    String getFormattedDate();
    
    time_t getCurrentEpochWithUtcOffset();

    void showTime(tm localTime);
    void showDate(tm localTime);
    void showEpoch();

  enum CitiesNTP : int {
      Melbourne = 0,
      Perth,
      Brisbane,
      Adelaide,
      Darwin,
      Hobart,
      Amsterdam,
      Athens,
      Barcelona,
      Berlin,
      Brussels,
      Budapest,
      Copenhagen,
      Dublin,
      Geneva,
      Helsinki,
      Kyiv,
      Lisbon,
      London,
      Madrid,
      Oslo,
      Paris,
      Prague,
      Roma,
      Moscow,
      StPetersburg,
      Stockholm,
      Auckland,
      Hawaii_Time,
      Alaska_Time,
      Pacific_Time,
      Mountain_Time,
      Mountain_Time_Arizona,
      Central_Time,
      Eastern_Time,
      Atlantic_Time,
      Jakarta,
      Jerusalem,
      Singapore,
      Ulaanbaatar, 
      Brazil,
      Argentina,
      Central_America
    };

    //NTP timezone setting
    String NTPTimeZones[43] = { "EST-10EDT-11,M10.5.0/02:00:00,M3.5.0/03:00:00", "WST-8", "EST-10",
                          "CST-9:30CDT-10:30,M10.5.0/02:00:00,M3.5.0/03:00:00", "CST-9:30",
                          "EST-10EDT-11,M10.1.0/02:00:00,M3.5.0/03:00:00", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "GMT+0IST-1,M3.5.0/01:00:00,M10.5.0/02:00:00", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00", "EET-2EEST,M3.5.0/3,M10.5.0/4",
                          "WET-0WEST-1,M3.5.0/01:00:00,M10.5.0/02:00:00", "GMT+0BST-1,M3.5.0/01:00:00,M10.5.0/02:00:00",
                          "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", "MSK-3MSD,M3.5.0/2,M10.5.0/3",
                          "MST-3MDT,M3.5.0/2,M10.5.0/3", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00",
                          "NZST-12NZDT-13,M10.1.0/02:00:00,M3.3.0/03:00:00", "HAW10", "AKST9AKDT",
                          "PST8PDT", "MST7MDT", "MST7", "CST6CDT", "EST5EDT", "AST4ADT", "WIB-7",
                          "GMT+2", "SGT-8", "ULAT-8ULAST,M3.5.0/2,M9.5.0/2", "BRST+3BRDT+2,M10.3.0,M2.3.0", "UTC+3", "CST+6"
                          };
};

#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include "TimeClient.h"
#include "FS.h"

#define HOSTNAME "NixieESP-" 
#define CONFIG "/conf.txt"
#define VERSION "1.01"

//******************************
// Start Settings
//******************************
// Webserver
String NixieHostname = "NixieESP-" + ESP.getChipId(); 
const int WEBSERVER_PORT = 80; // The port you can access this device on over HTTP
const boolean WEBSERVER_ENABLED = true;  // Device will provide a web interface via http://[ip]:[port]/
boolean IS_BASIC_AUTH = true;  // true = require athentication to change configuration settings / false = no auth
char* www_username = "admin";  // User account for the Web Interface
char* www_password = "password";  // Password for the Web Interface

// Date and Time
float UtcOffset = +1; // Hour offset from GMT for your timezone
boolean IS_24HOUR = true;     // 23:00 millitary 24 hour clock
int minutesBetweenDataRefresh = 15;

// Time 
TimeClient timeClient(UtcOffset);
long lastEpoch = 0;
long firstEpoch = 0;
long displayOffEpoch = 0;
String lastMinute = "xx";
String lastSecond = "xx";
String lastReportStatus = "";

// LED Settings
const int externalLight = LED_BUILTIN; // Set to unused pin, like D1, to disable use of built-in LED (LED_BUILTIN)

// OTA Updates
boolean ENABLE_OTA = true;     // this will allow you to load firmware to the device over WiFi (see OTA for ESP8266)
String OTA_Password = "";      // Set an OTA password here -- leave blank if you don't want to be prompted for password

//******************************
// End Settings
//******************************

String themeColor = "dark-grey"; // this can be changed later in the web interface.

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
///////////////////////////////////////
// OTA
#include <ESP8266mDNS.h>
#include "common.h"
#include "config.h"
#include "webserver.h"
#include "eeprom.h"
#include "wifiSetup.h"
const char* FileName = __FILE__;

void setup()
{
  delay(3000);
  initHardware();
  Serial.write(90);
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, LOW);
  //Serial.println("Ready!");

  if (loadSavedConfig())
  {
    if (checkWiFiConnection())
    {
      SETUP_MODE = false;
      startWebServer();
      //digitalWrite(ledPin5,HIGH);// Turn the status led Green when the WiFi has been connected
      return;
    }
  }
  SETUP_MODE = true;
  setupMode();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  if (WiFi.status() == WL_CONNECTED) {}
  if (SETUP_MODE)
  {
    DNS_SERVER.processNextRequest();
  }
  WEB_SERVER.handleClient();
  //yield();
  //heart();
}

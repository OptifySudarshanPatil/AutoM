#include "urlDecode.h"
extern const char* FileName;
String makePage(String title, String contents)
{
  String s = "";
  s += "<!DOCTYPE html><html>";
  s += "<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=utf-8\" > <meta name = \"viewport\" content = \"user-scalable=no, initial-scale=1.0, maximum-scale=1.0, width=device-width\">";
  s += "<meta name=\"apple-mobile-web-app-capable\" content=\"yes\" > <meta name=\"apple-mobile-web-app-status-bar-style\" content=\"black\" > <link rel=\"apple-touch-icon\" href=\"/m/images/iphone.png\" >";
  s += "<meta name=\"format-detection\" content=\"telephone=no\" > <link rel=\"stylesheet\" type=\"text/css\" href=\"http://fonts.googleapis.com/css?family=Allerta\"> <link href=\"/m/css/handheld.css\" rel=\"stylesheet\" type=\"text/css\" media=\"handheld\" >";
  s += "<link rel=\"stylesheet\" href=\"/m/css/ios.css\" media=\"screen\" > <script type=\"text/javascript\"> window.scrollTo(0,1); </script>";
  s += "<head>";
  s += "<title>";
  s += title;
  s += "</title></head>";
  s += "<body>";
  //s += "<header><p>" + DEVICE_TITLE + "</p></header>";
  s += "<div class=\"content-body\">";
  s += "<center>";
  s += contents;
  s += "</center>";
  s += "</div>";
  s += "</body></html>";
  return s;
}

void startWebServer()
{
  if (SETUP_MODE)
  {
    WEB_SERVER.on("/settings", []()
    {
      String s = "";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += SSID_LIST;
      s += "</select><br><br>Password: <input name=\"pass\" length=64 type=\"password\"><br><br><input type=\"submit\"></form>";
      WEB_SERVER.send(200, "text/html", makePage("Wi-Fi Settings", s));
      //WebServer.disconnect();
    });
    WEB_SERVER.on("/setap", []()
    {
      for (int i = 0; i < 96; ++i)
      {
        EEPROM.write(i, 0);
      }
      String ssid = urlDecode(WEB_SERVER.arg("ssid"));
      String pass = urlDecode(WEB_SERVER.arg("pass"));
      for (int i = 0; i < ssid.length(); ++i)
      {
        EEPROM.write(i, ssid[i]);
      }
      for (int i = 0; i < pass.length(); ++i)
      {
        EEPROM.write(32 + i, pass[i]);
      }
      EEPROM.commit();
      //Serial.println("Write EEPROM done!");
      String s = "<h1>WiFi Setup complete.</h1><p>The button will be connected automatically to \"";
      s += ssid;
      s += "\" after the restart.";
      delay(3000);
      ESP.restart();
    });

    WEB_SERVER.onNotFound([]()
    {
      String s = "";
      s += "<p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      //s += "<p><a href=\"/ota\">Firmware Upgrade</a></p>";
      WEB_SERVER.send(200, "text/html", makePage("Access Point mode", s));
    });
  }
  else
  {
    WEB_SERVER.on("/", []()
    {
      MyIp =  String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]);
      String s = "";
      //s += "<a href=\"/reset\"><button>RESET</button></br></a>";
      //s += "<a href=\"/switch1\"><button>Switch1</button></br>";
      //s += "<a href=\"/switch2\"><button>Switch2</button></br>";
      //s += firmware_version;
      s += FileName;  s += "</br>";
      s += __DATE__;  s += "</br>";
      s += __TIME__;  s += "</br>";
      s += MyIp;      s += "</br>";
      s += "<p><a href=\"/ota\"><button>Firmware Upgrade</button></br></a></p>";
      s += "<p><a href=\"/reset\"><button>RESET</button></br></a></p>";
      WEB_SERVER.send(200, "text/html", makePage("Station mode", s));
    });
    WEB_SERVER.on("/reset", []()
    {
      for (int i = 0; i < 96; ++i)
      {
        EEPROM.write(i, 0);
      }
      EEPROM.commit();
      String s = "<p>Device will reset now</p>";
      WEB_SERVER.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
      delay(2000);
    });
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// CONTROL
    WEB_SERVER.on("/switch1", []()
    {
      String s = "";
      /*
        if (state1 == HIGH) {
        state1 = LOW;
        s += "A";
        }
        else {
        state1 = HIGH;
        s += "a";
        }
        digitalWrite(ledPin1, state1);
        //Serial.println(state1);
        //s += "<a href=\"/switch1\"><button>Switch1</button></br>";
        //s += "<a href=\"/switch2\"><button>Switch2</button></br>";
        //WEB_SERVER.send(200, "text/html", makePage("Station mode", s));
      */
      digitalWrite(ledPin1, state1 = ~state1);
      WEB_SERVER.send(200, "text/html", makePage("Switch1", (digitalRead(ledPin1) == HIGH) ? (s += "A") : (s += "a")));
    });
   
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////OTA
    WEB_SERVER.on("/ota", HTTP_GET, []()
    {
      //OTA Request
      WEB_SERVER.sendHeader("Connection", "close");
      WEB_SERVER.sendHeader("Access-Control-Allow-Origin", "*");
      WEB_SERVER.send(200, "text/html", serverIndex);
    });


    WEB_SERVER.on("/update", HTTP_POST, []()
    {
      WEB_SERVER.sendHeader("Connection", "close");
      WEB_SERVER.sendHeader("Access-Control-Allow-Origin", "*");
      WEB_SERVER.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []()
    {
      HTTPUpload& upload = WEB_SERVER.upload();
      if (upload.status == UPLOAD_FILE_START)
      {
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        //Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace))
        { //start with max available size
          //Update.printError(Serial);
        }
      }
      else if (upload.status == UPLOAD_FILE_WRITE)
      {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
          //Update.printError(Serial);
        }
      }
      else if (upload.status == UPLOAD_FILE_END)
      {
        if (Update.end(true))
        { //true to set the size to the current progress
          //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        }
        else
        {
          //Update.printError(Serial);
        }
        //Serial.setDebugOutput(false);
      }
      yield();
    });
  }
  WEB_SERVER.begin();
}

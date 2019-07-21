
void setupMode()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  //Serial.println("");
  for (int i = 0; i < n; ++i)
  {
    SSID_LIST += "<option value=\"";
    SSID_LIST += WiFi.SSID(i);
    SSID_LIST += "\">";
    SSID_LIST += WiFi.SSID(i);
    SSID_LIST += "</option>";
  }
  delay(100);
  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(AP_IP, AP_IP, IPAddress(255, 255, 255, 0));
  //WiFi.softAP(AP_SSID,AP_PWD,1,1);//HIDDEN SSID
  WiFi.softAP(AP_SSID, AP_PWD, 1, 0); //VISIBLE SSID
  //WiFi.softAP(AP_SSID);           //OPEN SSID
  DNS_SERVER.start(53, "*", AP_IP);
  startWebServer();
  //Serial.print("Starting Access Point at \"");
  //Serial.print(AP_SSID);
  //Serial.println("\"");
  unsigned long currentMillis = millis();
  if (currentMillis > 180000) {
    ESP.restart();
  }
}

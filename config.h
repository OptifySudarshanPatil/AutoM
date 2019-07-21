boolean loadSavedConfig()
{
  String ssid = "";
  String password = "";
  if (EEPROM.read(0) != 0)
  {
    for (int i = 0; i < 32; ++i)
    {
      ssid += char(EEPROM.read(i));
    }
    //Serial.print("SSID: ");
    //Serial.println(ssid);
    for (int i = 32; i < 96; ++i)
    {
      password += char(EEPROM.read(i));
    }
    //Serial.print("Password: ");
    //Serial.println(password);
    // WiFi.begin(ssid.c_str(), password.c_str());

    //in case of err remove these 4 line
    IPAddress ip(192, 168, 1, 123);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.config(ip, gateway, subnet);
    return true;
  }
  else
  {
    //Serial.println("Saved Configuration not found.");
    return false;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean checkWiFiConnection()                   {
  int count = 0;
  //Serial.print("Waiting to connect to the specified WiFi network");
  while ( count < 30 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      //Serial.println("Connected!");
      return (true);
    }
    delay(500);
    //Serial.print(".");
    //if (state5 == HIGH)
    // state5 = LOW;
    // else
    //   state5 = HIGH;
    //digitalWrite(ledPin5, state5);
    count++;
  }
  //Serial.println("Timed out.");
  return false;
}

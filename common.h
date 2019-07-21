const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

const IPAddress AP_IP(192, 168, 1, 1);
const char* DEVICE_TITLE = "AutoM";
const char* firmware_version = "AUtoM_v3_17112016";
const char* AP_SSID = "AutoM";
const char* AP_PWD  = "c93pdunc";
boolean SETUP_MODE;
String SSID_LIST;
DNSServer DNS_SERVER;
ESP8266WebServer WEB_SERVER(80);

int ledPin1 = D5; // Relay4
int ledPin2 = D7; // Relay3
int ledPin3 = D6; // Relay2
int ledPin4 = D2;  // Relay1
//int ledPin5 = D4;  // D5 LED

int state1 = LOW;
int state2 = LOW;
int state3 = LOW;
int state4 = LOW;
//int state5 = LOW;

String fan1 ;
int fanspeed;
String  MyIp;
Encoder myEnc(D8, D1);
//Encoder myEnc(D2, D1);
long oldPosition  = 80;
long newPosition  = 80;

void initHardware()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(10);
}

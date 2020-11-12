#include "PID.h"
#include "ThingSpeak.h"
#include <WiFi.h>
#include "Secrets.h"

char ssid[] = SECRET_SSID;     //  your network SSID (name)
char pass[] = SECRET_PASS;    // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;


unsigned long ChannelID = SECRET_CH_ID;
const char * ReadAPIKey = SECRET_READ_APIKEY;
const char * WriteAPIKey = SECRET_WRITE_APIKEY;
unsigned int TempFieldNumber = 1;
unsigned int SpFieldNumber = 2;
unsigned int ControlSFieldNumber = 3;
unsigned int KpFieldNumber = 4;
unsigned int TiFieldNumber = 5;

PIDLib PID(false);

int count = 0;
int temppin = A0;
int PWMpin = 10;
//int PWM;
float V = 1;
float Kp = 1.15;
float Ts = 0.1;
float Ti = 9;
float r = 1;
float y;

float T_s = 0.1;
float T_f = 2;
float u2 = 0;


void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }
    
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  pinMode(PWMpin, OUTPUT);
}

void loop() {

  int statusCode = 0;
  
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected");
  }
  
  
  analogWrite(PWMpin, V*51);
  u2 = (analogRead(temppin)* 0.004882814);
  y = PID.filter(T_f,T_s,u2);
  V = PID.getControlSignal(Kp,Ts,Ti,r,y);
  Serial.println(V);

  delay(100);
  
  
  count = count +1;

  if (count > 300) {
    count = 0;}

  if (count == 20) {

    float Sp = ThingSpeak.readFloatField(ChannelID, SpFieldNumber, ReadAPIKey); 
    Kp = ThingSpeak.readFloatField(ChannelID, KpFieldNumber, ReadAPIKey);
    Ti = ThingSpeak.readFloatField(ChannelID, TiFieldNumber, ReadAPIKey);
    Serial.println(u2);
    Serial.println(y);

    Serial.println(Sp);
    Serial.println(Kp);
    Serial.println(Ti);
    
    r = ((Sp-20)*(5-1)/(50-20))+1;
    float Temp = ((y-1)*(50-20)/(5-1)) + 20;
  
    ThingSpeak.setField(1, Temp);
    ThingSpeak.setField(3, V);
    Serial.println(Temp);
    Serial.println(V);
    
    int x = ThingSpeak.writeFields(ChannelID, WriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
}

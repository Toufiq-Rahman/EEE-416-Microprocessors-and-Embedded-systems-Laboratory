

#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

//int number = 0;
int i;
String a;
float Vol;
float Curr;
float PF;
float period=20; //1/frequency of wave in milliseconds
//float Power;
//String b;
//String Vol;
//String Curr;
//int x;
void setup() {

  //pinMode(A0,INPUT);
  Serial.begin(9600);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  //String a;
  //String b;
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    //Serial.println("\nConnected.");
  }

  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  a.remove(0);
  while (a[0]!='$')
  {
      a.remove(0);
      if (Serial.available()>0){
          a=Serial.readStringUntil('\r');
          //b=Serial.readStringUntil('\r');
         
        }
  }

  a.remove(0,1);

  Vol = ((a.substring(0,4)).toFloat()/4095)*3.3*100;
  Curr =  ((a.substring(5,9)).toFloat() )/4095*3.3*10;
  PF = (a.substring(10)).toFloat();


  PF =PF/1000;
  if (PF>period/4)
  {
    PF = period/2-PF;
  }
  PF = cos((PF/period)*2*3.142) ;

  //Power = Vol*Curr;
  ThingSpeak.writeField(myChannelNumber, 1, Vol, myWriteAPIKey);
  delay(15000);
  ThingSpeak.writeField(myChannelNumber, 2, Curr, myWriteAPIKey);
  delay(15000); 
  ThingSpeak.writeField(myChannelNumber, 3, Vol*Curr, myWriteAPIKey);
  delay(15000);
  ThingSpeak.writeField(myChannelNumber, 4, PF, myWriteAPIKey);
  delay(15000);
  
}

#include <ESP8266WiFi.h>
#include "random.h"
#include "global.h"

/*++++++++++++++++Buffer declaration+++++++++++++++++*/
char ClientID[24];
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++Fn declaration++++++++++++++++++*/
void set_wifi(void);
char generateClientID (void); 
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++For initiating wifi connect+++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void set_wifi(void)
{
  Serial.println("WiFi Initiating |^^^-^^^| ");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); 
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  digitalWrite(2, HIGH);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(2, millis()>>8&1);  
    delay(250);
  }
  digitalWrite(2, LOW);
  Serial.println("WiFi connected");
}
/*++++++++++For creating random clientID ++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
char generateClientID (void) 
{
  memset(ClientID,0,sizeof(ClientID));
  char* cID = new char[23]();
  for (int i=0; i<22; i+=1)
    cID[i]=(char)random(1, 256);
  strcpy(ClientID,cID);
}

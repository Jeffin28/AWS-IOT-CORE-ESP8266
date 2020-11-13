#include <ESP8266WiFi.h>
#include "random.h"
#include "global.h"
#include "aws.h"

void setup() 
{
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    Serial.setDebugOutput(0); 
    generateClientID();
    init_AWS_service();
}

void loop() 
{
    if(WiFi.status() != WL_CONNECTED) 
    {
       set_wifi();
       AWS_Setup();
    }
    if(Finish==1)
    {
       Serial.printf("AWS_data %s\n",AWS_data);
       AWS_pub();
       Finish=0;
    }
    AWS_callback();
}

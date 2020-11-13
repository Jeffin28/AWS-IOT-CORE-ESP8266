#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <Stream.h>
#include "sha256.h"
#include "Utils.h"
#include <Hash.h>
#include <WebSocketsClient.h>
#include <PubSubClient.h>
#include "Client.h"
#include "AWSWebSocketClient.h"
#include "CircularByteBuffer.h"
#include "aws.h"
#include "random.h"

/*++++++++++++++++Buffer declaration+++++++++++++++++*/
char  AWS_data[256];
int   Finish=0;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++Credentials declaration+++++++++++++++*/
char aws_endpoint[]        = "YUR AWS ARN ENDPOINT";
char aws_key[]             = "YOUR ACCESS KEY ID";                             
char aws_secret[]          = "YOUR SECERET ACCESS KEY";                  
char aws_region[]          = "YOUR REGION";                            
const char* aws_pub_topic  = "YOUR PUBLISH TOPIC";  
const char* aws_sub_topic  = "YOUR SUBSCRIBE TOPIC";
int  port                  =  443;
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*++++++++++++++++++Obj declaration+++++++++++++++++++*/
AWSWebSocketClient awsWSclient(1000);
PubSubClient client(awsWSclient);
/*++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++Fn declaration+++++++++++++++++++++*/
void  init_AWS_service(void);
char  AWS_Setup(); 
void  AWS_sub(void); 
void  AWS_pub(void); 
void  AWS_callback(void);
/*++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*++++++++++++++++To init AWS ++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void init_AWS_service(void)
{
  awsWSclient.setAWSRegion(aws_region);
  awsWSclient.setAWSDomain(aws_endpoint);
  awsWSclient.setAWSKeyID(aws_key);
  awsWSclient.setAWSSecretKey(aws_secret);
  awsWSclient.setUseSSL(true);
}
/*+++++++++++++++To connect AWSIOT core +++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
char AWS_Setup(void) 
{ 
    client.disconnect ();
    Serial.print (ESP.getFreeHeap ());             
    client.setServer(aws_endpoint, port);
    client.setCallback(callback);
    int ret = client.connect(ClientID);
    Serial.printf("client_Connect %d\n",ret); 
    
    switch(ret)
    {
        case SUCCESS          : Serial.printf("Connected clientID : %s\n",ClientID);   
                                AWS_sub();  
                                AWS_pub();
                                return 1;
                                break; 
                             
        case TIMED_OUT        :
        case TRUNCATED        :
        case INVALID_RESPONSE :
        case INVALID_SERVER   : Serial.println("CONNECTION ERROR");   
                                return 0;
                                break;  
        default               : Serial.printf("UNAUTHORISED ERROR CODE %d\n",client.state());
                                return 0;
                                break;                                                       
      
    } 
}
/*+++++++++++++For Subscribing AWS data++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void AWS_sub(void) 
{
  client.subscribe(aws_sub_topic);
  Serial.println("AWS Subscribed");
}
/*+++++++++++++For publishing AWS data+++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void AWS_pub(void) 
{   
  client.publish(aws_pub_topic,"Data Published"); 
  Serial.println("AWS Published");
}
/*+++++++++++++For Callback AWS data+++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void callback(char* topic, byte* payload, unsigned int length) 
{
  Finish = 0;
  memset(AWS_data,0,sizeof(AWS_data));
  Serial.print("Message arrived [");
  Serial.printf("topic : %s\n",topic);
  Serial.print("] ");
  if(!strcmp(topic,"incomming"))
  {
    for (int i = 0; i < length; i++)
     AWS_data[i] = payload[i];
    Finish=1;
  }
}
/*+++++++++++++++For AWS callback++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void AWS_callback(void)
{
   if(awsWSclient.connected())    
      client.loop();
   else
   { 
      if(AWS_Setup())
        AWS_sub();  
   }
}

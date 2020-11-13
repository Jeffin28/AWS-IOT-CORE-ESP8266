#ifndef __AWS_H
#define __AWS_H

#define SUCCESS           1
#define TIMED_OUT        -1
#define INVALID_SERVER   -2
#define TRUNCATED        -3
#define INVALID_RESPONSE -4 

const int maxMQTTpackageSize = 512;
const int maxMQTTMessageHandlers = 1;

extern int  Finish;
extern char AWS_data[256];

extern void init_AWS_service(void);
extern char AWS_Setup(); 
extern void callback(char* topic, byte* payload, unsigned int length); 
extern void AWS_callback(void);
extern void AWS_pub(void);
extern void AWS_sub(void); 

extern "C"
{
  #include "user_interface.h"
}

#endif

#ifndef _IFTTT_MESSAGE_H
#define _IFTTT_MESSAGE_H

#include <Arduino.h>

#include <WiFiServerSecure.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiType.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFiAP.h>
#include <WiFiClient.h>
#include <BearSSLHelpers.h>
#include <WiFiServer.h>
#include <ESP8266WiFiScan.h>
#include <WiFiServerSecureBearSSL.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiSTA.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiServerSecureAxTLS.h>


#include "MessageSender.h"

class IFTTTMessageClass : MessageSenderClass
{
  private:
    char* APIKey;
    WiFiClient TheClient;
    
    // String used for static parts of message sent to IFTTT
    String PostString;    
    
    // String used to hold the first part of the JSON packet, typically a
    // unique identifier for the sensor host
    String TheTag;
    
    // Connect to the ifttt service. Returns true if connection was successful
    virtual bool Connect (void);



  public:
   // Constructor - pass in API key for IFTTT and a tag to use in the JSON packet,
   // which is typically a unique identifier for this host
   IFTTTMessageClass (const char* theAPIKey, char* theTag);

    // Send a message. Return value indicates whether or not message was successfully sent
    virtual bool Send (String theMessage);

};

#endif

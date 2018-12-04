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

  public:
   IFTTTMessageClass (const char* theAPIKey);

    // Connect to the ifttt service.
    virtual void Connect (void);

    // Send a message. Return value indicates whether or not message was successfully sent
    virtual bool Send (String theMessage);

};

#endif

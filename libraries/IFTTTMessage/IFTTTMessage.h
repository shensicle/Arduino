#ifndef _IFTTT_MESSAGE_H
#define _IFTTT_MESSAGE_H

#include <WiFi.h>
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
    virtual boolean Send (String theMessage);

};

#endif

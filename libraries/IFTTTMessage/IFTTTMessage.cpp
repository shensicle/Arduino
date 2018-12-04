
#include "IFTTTMessage.h"

// -----------------------------------------------------
IFTTTMessageClass::IFTTTMessageClass (const char* theAPIKey) : MessageSenderClass()

{
    APIKey = (char*)theAPIKey;
}

// -----------------------------------------------------
// Connect to the ifttt service.
void IFTTTMessageClass::Connect (void)
{
   Serial.println("Connecting to IFTTT...");
   if(TheClient.connect("maker.ifttt.com",80))  // Test the connection to the server
   {
     Serial.println("Connected to Maker");
   }
   else
   {
     Serial.println("Failed to connect to Maker.");
   }
}

// -----------------------------------------------------
// Send a message. Return value indicates whether or not message was successfully sent
bool IFTTTMessageClass::Send (String theMessage)
{
    // Packet will end up looking similar to this:
  // 
  //  POST /trigger/ESP/with/key/(myKey) HTTP/1.1
  //  Host: maker.ifttt.com
  //  User-Agent: ESP32_Thing
  //  Connection: close
  //  Conent-Type: application/json
  //  Conent-Length: (postData length)
  //  
  //  {"value1":"(HIGH/LOW )"}
  
  String postData = "{\"value1\":\""; // JSON value to send in SMS message
 // if(waterLevel) postData.concat("HIGH");
 // else postData.concat("LOW ");
  postData.concat("\"}");
    
/*
TheClient.print("POST /trigger/");
  TheClient.print("ESP");
  TheClient.print("/with/key/");
  TheClient.print(APIKey);
  TheClient.println(" HTTP/1.1");

  TheClient.println("Host: maker.ifttt.com");
  TheClient.println("User-Agent: WaterSensor");
  TheClient.println("Connection: close");

  TheClient.println("Content-Type: application/json");
  TheClient.print("Content-Length: ");
  TheClient.println(postData.length());
  TheClient.println();
  TheClient.println(postData);
  */
}

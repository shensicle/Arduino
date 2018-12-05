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


#include <MessageSender.h>

#include <IFTTTMessage.h>

#include <WaterSensor.h>


#include "WaterSensor.h"



// --------------------------------------------------------------------------------------------------

// Wifi stuff
const char* wifiSSID      = "OVIS-4FEF-2.4G";      // Your router's ssid goes here
const char* wifiPassword  = "BR21706502790";  // Password to router goes here

// ifttt stuff
const char* iftttKey      = "bdf53ikwIP3q107qlHt6s4"; // Key for ifttt.com API
IFTTTMessageClass IFTTTSender(iftttKey, "Sensor1");   // Communicates with ifttt.com

// Water sensor stuff
const int  theSensorPin  = A0;                             // Analog IO pin connected to water level sensor


WaterDetectState CurrentState = WaterDetectState::NO_SENSOR_DETECT,
                 PreviousState = WaterDetectState::NO_SENSOR_DETECT;
      

// Need to make this
WaterSensorClass theWaterSensor (theSensorPin); // constructor initializes pin, initializes everything else


// --------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); // Start serial communication for debug information
  
  ConnectWifi((char*)wifiSSID, (char*)wifiPassword);        // Connect to WiFi
}

// --------------------------------------------------------------------------------------------------
void loop()
{  
  
  // If  we're connected to Wifi ...
  if(WiFi.status() != WL_CONNECTED) 
  {
      ConnectWifi((char*)wifiSSID, (char*)wifiPassword);  // Connect to WiFi  
  }
  
  else  // We are connected to Wifi. Read sensors and deal with the result
  {
    ServiceWaterSensor();

    // ServiceTemperatureSensor();

    // ServiceTerminal();   // Need to do this more often than every 10 seconds
    
  }
  

  // Wait 10 seconds before doing anything again
  delay (10000);

}


// --------------------------------------------------------------------------------------------------
bool ConnectWifi(char* ssid, char* password)  // Tries to connect to the wireless access point with the credentials provided
{
  bool timeOut = 0; // Change to 1 if connection times out
  byte attempts = 0;   // Counter for the number of attempts to connect to wireless AP
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password); // Connect to WiFi network

  while (WiFi.status() != WL_CONNECTED && (timeOut == 0)) // Test to see if we're connected
  {
    Serial.print('.');
    attempts++;
    
    if(attempts > 60) break; // Give up after ~30 seconds
    else delay(500);      // Check again after 500ms
  }
  
  if (WiFi.status() == WL_CONNECTED)  // We're connected
  {
    Serial.println("\r\nWiFi connected");
  }
  else  // Unable to connect
  {
    WiFi.disconnect();
    Serial.println("\r\nConnection Timed Out!\r\n");
  }
}

// --------------------------------------------------------------------------------------------------
void ServiceWaterSensor(void)
{
    // Poll the sensor. There's a small chance that water could rise from
    // zero to deep right after the read, but since I'm using this to monitor a basement,
    // that would likely require a tsunami.
    CurrentState = theWaterSensor.ReadSensor ();


    // Now run through the FSM. We don't need to do anything if the state hasn't changed since last time
    if (CurrentState != PreviousState)
    {
      switch (CurrentState)
      {
       case WaterDetectState::NO_SENSOR_DETECT:
            // Send a message indicating flooding has subsided
            IFTTTSender.Send ("No water detected");
            break;
            
      case WaterDetectState::SHALLOW_SENSOR_DETECT:
        switch (PreviousState)
        {
          case WaterDetectState::NO_SENSOR_DETECT:  
           // Send message indicating flooding may have started
           IFTTTSender.Send ("Water detected at shallow level");
           break;
           
           case WaterDetectState::DEEP_SENSOR_DETECT:
              // Message message water level going down
              IFTTTSender.Send ("Water has gone down to shallow level");
              break;

            default:
              // Message software error
              IFTTTSender.Send ("Software Error: 1");
              break;

        } // switch PreviousState
        break; // switch CurrentState
          
 
        case WaterDetectState::DEEP_SENSOR_DETECT:
          switch (PreviousState)
          {
            case WaterDetectState::SHALLOW_SENSOR_DETECT:
              // Message flooding is getting worse
              IFTTTSender.Send ("Water has risen to deep level");
              break;

          case WaterDetectState::NO_SENSOR_DETECT:  
              // Send message indicating fast flooding may have started 
              IFTTTSender.Send ("Water fast rise to deep level");
           
           break;

            default:
              // Message possible system fault
              IFTTTSender.Send ("Software Error: 2");
              break;
              
          } // switch PreviousState
          break;  //switch CurrentState

        default:
          // Message software error
          IFTTTSender.Send ("Software Error: 3");

          break;  //switch CurrentState
      } // switch CurrentState

      // Update the previous state for next time
      PreviousState = CurrentState;
      
    } // if CurrentState != PreviousState
}

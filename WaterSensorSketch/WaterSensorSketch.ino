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
#include "PersonalityDefs.h"
#include <Personality.h>

// Constants we use in the loop() method to serivce hardware at different intervals
const unsigned int delayPerLoop = 500;   //milliseconds
const unsigned int ledUpdateCount = 1;   // update LED every half second
const unsigned int waterSensorUpdateCount =  20;  // check water sensor every 10 seconds
const unsigned int tempSensorUpdateCount  = 20;   // check temperature sensor every 10 seconds

// --------------------------------------------------------------------------------------------------

IFTTTMessageClass IFTTTSender;   // Communicates with ifttt.com

// Structure that stores our personality
personality_t pers;



// Water sensor stuff
const int  theSensorPin  = A0;        // Analog IO pin connected to water level sensor

// Buzzer stuff
//const int theBuzzerPin = D4;          // Digital IO pin for buzzer

WaterDetectState CurrentState = WaterDetectState::NO_SENSOR_DETECT,
                 PreviousState = WaterDetectState::NO_SENSOR_DETECT;
      

// Need to make this
WaterSensorClass theWaterSensor (theSensorPin); // constructor initializes pin, initializes everything else


// --------------------------------------------------------------------------------------------------
void setup() {

  // Start serial communication for debug information
  Serial.begin(115200); 

  // Clear personality structure, just because
  memset (&pers, 0, sizeof (pers));

  // Object to read personality data from EEPROM
  PersonalityClass persReader;

  bool okay = persReader.Read(&pers);
  if (okay == true)
  {
    Serial.print ("\nChecksum is "); Serial.println (okay);
    Serial.print ("Sensor ID: "); Serial.println (pers.UUID);

   // We can now initialize fields to be sent to IFTTT that were in the personality
   IFTTTSender.Initialize (pers.IFTTTKey, pers.UUID);

   // And if we have a buzzer, set it up too
// if (pers.HasBuzzer)
//     pinMode(buzzerPin, OUTPUT);

       // Now connect to Wifi for the first time and send a startup message
       ConnectWifi(pers.WifiSSID, pers.WifiPassword);  // Connect to WiFi 

       IFTTTSender.Send ("Device startup");
  }

  else
  {
     Serial.println ("*** Personality checksum failure ***");

     // No point in continuing
     while (1)
       delay (500);
  }
}

// --------------------------------------------------------------------------------------------------
void loop()
{  
  // Count each loop to determine when we're supposed to service the various hardware devices
  static unsigned int loopCounter = 0;
  
  // If  we're connected to Wifi ...
  if(WiFi.status() != WL_CONNECTED) 
  {
      ConnectWifi(pers.WifiSSID, pers.WifiPassword);  // Connect to WiFi  
  }
  
  else  // We are connected to Wifi. Read sensors and deal with the result
  {

    if ((loopCounter % waterSensorUpdateCount) == 0)
      ServiceWaterSensor();

//  if ((loopCounter % tempSensorUpdateCount) == 0)
    // ServiceTemperatureSensor();

    if ((loopCounter % ledUpdateCount) == 0)
      ServiceLED ();

    loopCounter ++;
  }
  
  // Wait 10 seconds before doing anything again
  delay (delayPerLoop);

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
              IFTTTSender.Send ("Software Error: Water Sensor 1");
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
              IFTTTSender.Send ("Software Error: Water Sensor 2");
              break;
              
          } // switch PreviousState
          break;  //switch CurrentState

        default:
          // Message software error
          IFTTTSender.Send ("Software Error: Water Sensor 3");

          break;  //switch CurrentState
      } // switch CurrentState

      // Update the previous state for next time
      PreviousState = CurrentState;
      
    } // if CurrentState != PreviousState
}

// --------------------------------------------------------------------------------------------------
void ServiceLED (void)
{
}

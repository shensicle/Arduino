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
const unsigned int DelayPerLoop = 500;           // milliseconds
const unsigned int LedUpdateCount = 1;           // update LED every half second
const unsigned int WaterSensorUpdateCount = 20;  // check water sensor every 10 seconds
const unsigned int TempSensorUpdateCount  = 20;  // check temperature sensor every 10 seconds

// Flag which, when set, indicates that a device startup notification has been sent to ifttt.com. An attempt
// is made to send at startup, but if power to the wifi router was also interrupted, it will not likely be
// operational before this device is up and running.
bool StartupMessageSent = false;

// --------------------------------------------------------------------------------------------------

IFTTTMessageClass IFTTTSender;   // Communicates with ifttt.com

// Structure that stores our personality
personality_t ThePersonality;


// Water sensor stuff
const int  TheSensorPin  = A0;        // Analog IO pin connected to water level sensor

// Buzzer stuff
//const int TheBuzzerPin = D4;          // Digital IO pin for buzzer

// LED stuff - use built-in LED connected to D2
const int TheLEDPin = LED_BUILTIN;


// --------------------------------------------------------------------------------------------------
void setup() {

  // Start serial communication for debug information
  Serial.begin(115200); 

  // Set up control pin for LED
  pinMode (TheLEDPin, OUTPUT);

  // Clear personality structure, just because
  memset (&ThePersonality, 0, sizeof (ThePersonality));

  // Object to read personality data from EEPROM
  PersonalityClass persReader;

  // Read personaility and validate the checksum
  bool okay = persReader.Read(&ThePersonality);

  // If the personality checksum test passed
  if (okay == true)
  {
    Serial.print ("\nChecksum is "); Serial.println (okay);
    Serial.print ("Sensor ID: "); Serial.println (ThePersonality.UUID);

    // We can now initialize fields to be sent to IFTTT that were in the personality
    IFTTTSender.Initialize (ThePersonality.IFTTTKey, ThePersonality.UUID);

   // And if we have a buzzer, set it up too
// if (ThePersonality.HasBuzzer)
//     pinMode(TheBuzzerPin, OUTPUT);

    // Now connect to Wifi for the first time and send a startup message
    ConnectWifi(ThePersonality.WifiSSID, ThePersonality.WifiPassword);  // Connect to WiFi 

    // If wifi connected, send device startup message
    if (WiFi.status() == WL_CONNECTED)
    {
        IFTTTSender.Send ("Device startup");
         StartupMessageSent = true;           // don't want to do this again until next power up
    }
  }

  else
  {
      Serial.println ("*** Personality checksum failure ***");

      // No point in continuing
      while (1)
      {
          ServiceLED();
          delay (DelayPerLoop/8);  // Flash LED 8 times faster when in fault mode
      } 
  }
}

// --------------------------------------------------------------------------------------------------
void loop()
{  
  // Count each loop to determine when we're supposed to service the various hardware devices
  static unsigned int loopCounter = 0;
  
  // If  we're not connected to Wifi ...
  if (WiFi.status() != WL_CONNECTED) 
  {
      ConnectWifi(ThePersonality.WifiSSID, ThePersonality.WifiPassword);  // Connect to WiFi

      // If we are now connected and this is the first successful connect, send a device startup message. This handles the
      // case where power to the whole house goes off and this device comes up before the Wifi does
      if ((WiFi.status() == WL_CONNECTED) && (StartupMessageSent == false))
      {
          StartupMessageSent = true;
          IFTTTSender.Send ("Device startup (delayed waiting for Wifi)");
      }

  }
  
  else  // We are connected to Wifi. Read sensors and deal with the result
  {

    if (ThePersonality.HasWaterSensor && ((loopCounter % WaterSensorUpdateCount) == 0))
      ServiceWaterSensor();

    if (ThePersonality.HasTempSensor && ((loopCounter % TempSensorUpdateCount) == 0))
      ServiceTemperatureSensor();

    if ((loopCounter % LedUpdateCount) == 0)
      ServiceLED ();

    loopCounter ++;
  }
  
  // Wait 10 seconds before doing anything again
  delay (DelayPerLoop);

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
    
        if(attempts > 60) 
            break; // Give up after ~30 seconds
        else 
            delay(500);      // Check again after 500ms
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
    static WaterDetectState CurrentState = WaterDetectState::NO_SENSOR_DETECT,
                 PreviousState = WaterDetectState::NO_SENSOR_DETECT;
      
    // Manage the water sensor
    static WaterSensorClass TheWaterSensor (TheSensorPin);


    // Poll the sensor. There's a small chance that water could rise from
    // zero to deep right after the read, but since I'm using this to monitor a basement,
    // that would likely require a tsunami.
    CurrentState = TheWaterSensor.ReadSensor ();


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
  static int ledState = 0;

  digitalWrite (TheLEDPin, ledState);
  ledState = 1 - ledState;
  
}

// --------------------------------------------------------------------------------------------------
void ServiceTemperatureSensor (void)
{
  
}

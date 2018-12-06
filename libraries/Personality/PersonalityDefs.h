#ifndef _PERSONALITYDEFS_H
#define _PERSONALITYDEFS_H


// Structure to save a personality file for this sketch in EEPROM
typedef struct
{
      char WifiSSID[20];
      char WifiPassword[20];
      char IFTTTKey[30];  
      char UUID[20];
      bool HasWaterSensor;
      bool HasTempSensor;
      bool HasBuzzer;
} personality_t;


#endif

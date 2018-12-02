#ifndef _WATERSENSOR_H
#define _WATERSENSOR_H


// Class to define possible sensor states
enum class WaterDetectState {NO_SENSOR_DETECT, SHALLOW_SENSOR_DETECT, DEEP_SENSOR_DETECT};


class WaterSensorClass
{
  private:
      unsigned char SensorPin;
            
      
  public: 
  	  
    WaterSensorClass (const unsigned char sensorPin);

    // Read the sensor and return the result
    WaterDetectState ReadSensor (void);

};

#endif

#ifndef _WATERSENSOR_H
#define _WATERSENSOR_H


// Class to define possible sensor states
enum class WaterDetectState {NO_SENSOR_DETECT, SHALLOW_SENSOR_DETECT, DEEP_SENSOR_DETECT};


class WaterSensorClass
{
  private:
      unsigned char SensorPin;
	
      // Limit for water depth. Sensor range is 0 - 1023 (dry - wet)
	  int ShallowWaterLimit;
      int DeepWaterLimit;
            
      
  public: 
  	  
    WaterSensorClass (const unsigned char sensorPin,
    	              int shallowWaterLimit = 200,
    	              int deepWaterLimit    = 400);

    // Read the sensor and return the result
    WaterDetectState ReadSensor (void);
    
    // Set water depth limits due to user change at runtime
    void SetShallowWaterLimit (int theLimit) {ShallowWaterLimit = theLimit;}
    void SetDeepWaterLimit (int theLimit)    {DeepWaterLimit = theLimit;}

};

#endif

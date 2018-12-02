#include <Arduino.h>

#include "WaterSensor.h"

WaterSensorClass::WaterSensorClass (const unsigned char sensorPin)
{
  SensorPin = sensorPin;
}

// -----------------------------------------------------------
// Read the sensor and store result in WaterIsDetected
WaterDetectState WaterSensorClass::ReadSensor (void)
{    
	
	// Limit for shallow water 
	const int shallowWaterLimit = 900;
	
	// Highest value sensor can return (when dry)
	const int fullScaleReading = 1023;
	
	WaterDetectState returnValue = WaterDetectState::NO_SENSOR_DETECT;
	
	// Read the analog pin - returns a value between 0 and maxSensorReading
	int analogValue = analogRead(SensorPin);
	
	// Analog pin returns full voltage when dry
	if (analogValue != fullScaleReading)
	{
		// If we got here, we're wet
		if (analogValue > shallowWaterLimit)
			returnValue = WaterDetectState::DEEP_SENSOR_DETECT;
		else
			returnValue = WaterDetectState::SHALLOW_SENSOR_DETECT;
	}	
	return (returnValue);
}

#include <Arduino.h>

#include "WaterSensor.h"

WaterSensorClass::WaterSensorClass (const unsigned char sensorPin,
	                                int shallowWaterLimit,
	                                int deepWaterLimit)
{
  SensorPin = sensorPin;
  ShallowWaterLimit = shallowWaterLimit;
  DeepWaterLimit = deepWaterLimit;
}

// -----------------------------------------------------------
// Read the sensor and store result in WaterIsDetected
WaterDetectState WaterSensorClass::ReadSensor (void)
{    
	// Return value initialized to most likely state
	WaterDetectState returnValue = WaterDetectState::NO_SENSOR_DETECT;
	
	// Read the analog pin - returns a value between 0 and maxSensorReading
	int analogValue = analogRead(SensorPin);
	
	// Analog pin returns higher value when sensor is wet
	if (analogValue > ShallowWaterLimit)
	{
		// If we got here, we're wet
		if (analogValue > DeepWaterLimit)
			returnValue = WaterDetectState::DEEP_SENSOR_DETECT;
	}

	return (returnValue);
}

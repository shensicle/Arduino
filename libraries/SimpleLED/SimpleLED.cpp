/*
Copyright © 2011 Scott Henwood/shensicle photographic. All Rights Reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

3. The name of the author may not be used to endorse or promote products 
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Scott Henwood/shensicle photographic "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <SimpleLED.h>
	
// --------------------------------------------------------------
SimpleLED::SimpleLED (	byte outputPin,
			bool isActiveHigh,
			unsigned long updateInterval, // milliseconds
			unsigned long flashDuration)  // milliseconds
: TwoStateOutput (outputPin, isActiveHigh)
{
		// Save parameters
		IsLowPowerMode = false;
		UpdateInterval = updateInterval;
		
		// Don't let flash duration be less than 100 milliseconds
		FlashDuration  = flashDuration;
		if (FlashDuration < 100)
		{
			FlashDuration = 100;
		}
		
		// Initialize other variables
		CurrentState = OFF;
		FlashMode = false;
		TimeToNextStateChange = 0;
}

	
// --------------------------------------------------------------
// Method to enable/disable flashing
void SimpleLED::SetFlashMode (bool flashEnabled)
{
	
	// if we are not currently flashing ...
	if (FlashMode == false)
	{
		// if we're supposed to enable flashing ...
		if (flashEnabled == true)
		{
			FlashMode = true;
			TimeToNextStateChange = FlashDuration;
			SetOn();
		}
	}
	else // we are currently flashing
	{
		// If we're supposed to stop flashing ...
		if (flashEnabled == false)
		{
			FlashMode = false;
			SetOff();
		}
	}
}


// --------------------------------------------------------------
// Method to determine whether or not we are in low power mode. When in
// low power mode, LEDs only stay on briefly when turned on.
void SimpleLED::SetLowPowerMode (bool isEnabled)
{
	IsLowPowerMode = isEnabled;
}

// --------------------------------------------------------------
// Method called by the controlling program at UpdateInterval to allow
// us to manage the LED. Returns true if we are still running after
// the call and false otherwise.
bool SimpleLED::Update (void)
{
	// If we are flashing OR
	// If we're in low power mode and LED is currently on ...
	if ((FlashMode == true) || (CurrentState == ON && IsLowPowerMode == true))
	{
		// If it's time to change the state
		if (TimeToNextStateChange <= UpdateInterval)
		{
			if (CurrentState == ON)
			{
				SetOff();
			}
			else
			{
				SetOn();
			}
			
			TimeToNextStateChange = FlashDuration;
		}
		else // not time to do anything yet
		{
			TimeToNextStateChange -= UpdateInterval;

		}
	}
}

// --------------------------------------------------------------
void SimpleLED::SetOn (void)
{
	TwoStateOutput::SetOn();
	CurrentState = ON;
	TimeToNextStateChange = FlashDuration;
}

// --------------------------------------------------------------
void SimpleLED::SetOff (void)
{
	TwoStateOutput::SetOff();
	CurrentState = OFF;
}


